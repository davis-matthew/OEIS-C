/*
 * A074301  Numbers n such that (i) the largest prime factor of n is not a
 *          palindrome and (ii) the sum of the factorials of the digits of n
 *          is equal to the largest prime factor of n reversed.
 * https://oeis.org/A074301
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 21, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   gpf, is_palindrome, rev_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * This implementation computes OEIS A074301.
 * Sequence definition:
 *   a(n) is the nth positive integer such that
 *     (i) its largest prime factor is not a palindrome, and
 *     (ii) the sum of the factorials of its decimal digits equals
 *         the decimal reverse of that largest prime factor.
 *
 * No closed form is known; we generate terms sequentially, caching the
 * results so each term is computed only once.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/gpf.h"        /* uint64_t gpf(uint64_t x);          */
#include "../utils/is_palindrome.h"  /* bool is_palindrome(seq_t, idx_t);  */
#include "../utils/rev_decimal.h"    /* seq_t rev_decimal(seq_t);         */

static const seq_t fact[10] = {1, 1, 2, 6, 24, 120, 720, 5040, 40320, 362880};

static seq_t *terms   = NULL;  /* cached sequence values       */
static size_t terms_len = 0;    /* number of values stored      */
static size_t terms_cap = 0;    /* allocated capacity           */

/* Ensure that the cache can hold at least 'need' terms. */
static void ensure_capacity(size_t need)
{
    if (need <= terms_cap)
        return;
    size_t new_cap = terms_cap ? terms_cap * 2 : 16;
    if (new_cap < need)
        new_cap = need;
    void *tmp = realloc(terms, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                 /* malloc failure */
    terms = (seq_t *)tmp;
    terms_cap = new_cap;
}

/* Append a new term to the cache. */
static void append_term(seq_t val)
{
    ensure_capacity(terms_len + 1);
    terms[terms_len++] = val;
}

/* Test whether an integer n satisfies the two defining properties. */
static bool is_valid_uint64(uint64_t n)
{
    if (n < 2)
        return false;

    /* (i) largest prime factor is not a palindrome */
    uint64_t lpf_val = gpf(n);                      /* greatest prime factor */
    seq_t lpf = (seq_t)lpf_val;
    if (is_palindrome(lpf, 10))
        return false;

    /* (ii) sum of factorials of digits equals reverse(lpf) */
    seq_t sum = 0;
    uint64_t t = n;
    while (t > 0) {
        int d = (int)(t % 10);
        sum += fact[d];
        t /= 10;
    }

    seq_t rev_lpf = rev_decimal(lpf);
    return sum == rev_lpf;
}

/* Generate terms until at least 'n' terms are known. */
static void grow_until(idx_t n)
{
    if (n == 0)
        return;
    size_t target = (size_t)n;

    static uint64_t next_candidate = 2; /* smallest n that could be valid */
    while (terms_len < target) {
        if (is_valid_uint64(next_candidate))
            append_term((seq_t)next_candidate);
        ++next_candidate;
        if (next_candidate == 0)          /* overflow guard */
            break;
    }
}

/* Public function required by the judge. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    grow_until(n);
    return terms[n - 1];
}
