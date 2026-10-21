/*
 * A084992  Palindromes such that the difference between the consecutive terms
 *          are palindromes and this has not been the difference of any two
 *          consecutive terms earlier.
 * https://oeis.org/A084992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 28, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   is_palindrome, pow10, rev_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A084992: palindromes whose consecutive differences are palindromes and each
 *   difference has not appeared before.  The function generates palindromes
 *   in increasing order, keeps the set of already used differences, and
 *   returns the n‑th term, for n ≥ 1. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/is_palindrome.h"
#include "../utils/rev_decimal.h"
#include "../utils/pow10.h"

static const size_t INIT_CAP = 64;              /* initial cache capacity */

/* ---------- cache for the sequence terms ---------- */
static seq_t *term_cache  = NULL;              /* array of terms */
static size_t cache_len   = 0;                  /* number of terms stored   */
static size_t cache_cap   = 0;                  /* allocated capacity       */
static seq_t prev_term    = 0;                  /* last generated term      */

/* ---------- array of used differences ---------- */
static seq_t *diff_used   = NULL;              /* array of differences     */
static size_t diff_len    = 0;                  /* number of differences    */
static size_t diff_cap    = 0;                  /* allocated capacity       */

/* Append a new difference to the list of used differences. */
static void diff_insert(seq_t diff)
{
    if (diff_len == diff_cap) {
        diff_cap = (diff_cap == 0) ? INIT_CAP : diff_cap * 2;
        diff_used = (seq_t *)realloc(diff_used, diff_cap * sizeof(seq_t));
    }
    diff_used[diff_len++] = diff;
}

/* Test whether a difference has already appeared. */
static bool diff_contains(seq_t diff)
{
    for (size_t i = 0; i < diff_len; ++i)
        if (diff_used[i] == diff) return true;
    return false;
}

/* ---------- generation of the next palindrome ---------- */
static seq_t next_palindrome(void)
{
    static bool first = true;
    static idx_t curr_len = 1;      /* current number of digits */
    static seq_t left, start, end; /* current and bounds for the left half */

    if (first) {
        /* Initialize the state for generating palindromes. */
        left  = (seq_t)2;                       /* skip the pal 1 */
        curr_len = 1;
        /* Compute start = 10^(half_len-1), end = 10^half_len-1 */
        idx_t half_len = (curr_len + 1) / 2;
        start = pow10(half_len - 1);
        end   = pow10(half_len) - (seq_t)1;
        first = false;
    }

    /* If we've exhausted the current length, move to the next length. */
    if (left > end) {
        ++curr_len;
        idx_t half_len = (curr_len + 1) / 2;
        start = pow10(half_len - 1);
        end   = pow10(half_len) - (seq_t)1;
        left  = start;
    }

    seq_t pal;
    seq_t pow = pow10(curr_len / 2);

    if (curr_len % 2 == 0) {              /* even length */
        seq_t rev = rev_decimal(left);
        pal = left * pow + rev;
    } else {                              /* odd length */
        seq_t rev = rev_decimal(left / (seq_t)10);
        pal = left * pow + rev;
    }

    left = left + (seq_t)1;                /* advance left half */
    return pal;
}

/* ---------- main sequence function ---------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined for n=0, return 0 */

    /* Initialise caches on first call. */
    if (cache_cap == 0) {
        cache_cap = INIT_CAP;
        term_cache = (seq_t *)calloc(cache_cap, sizeof(seq_t));
        diff_cap   = INIT_CAP;
        diff_used  = (seq_t *)calloc(diff_cap, sizeof(seq_t));

        /* First term is 1. */
        term_cache[0] = (seq_t)1;
        cache_len = 1;
        prev_term = (seq_t)1;
    }

    /* Generate terms until we reach the requested index. */
    while (cache_len < n) {
        seq_t cand = next_palindrome();
        seq_t diff = cand - prev_term;

        if (is_palindrome(diff, 10) && !diff_contains(diff)) {
            if (cache_len == cache_cap) {
                cache_cap = cache_cap * 2;
                term_cache = (seq_t *)realloc(term_cache, cache_cap * sizeof(seq_t));
            }
            term_cache[cache_len] = cand;
            ++cache_len;
            prev_term = cand;
            diff_insert(diff);
        }
    }
    return term_cache[n - 1];
}
