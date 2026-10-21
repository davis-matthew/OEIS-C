/*
 * A209875  Primes p such that p and p+18 are consecutive primes with equal
 *          digital sum.
 * https://oeis.org/A209875
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 201, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_sum, next_prime, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A209875: primes p such that p and p+18 are consecutive primes
 * with equal digital sum.  The function uses an incremental search
 * combined with caching to handle arbitrary call orders.  */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>

#include "../utils/next_prime.h"   /* provides: uint64_t next_prime(uint64_t start); */
#include "../utils/digit_sum.h"    /* provides: seq_t digit_sum(seq_t v); */
#include "../utils/seq_realloc.h"  /* provides: seq_t* seq_realloc(seq_t* old, size_t old_cap, size_t new_cap); */

/* Static cache for the sequence */
static seq_t *seq_terms   = NULL;
static size_t seq_cap     = 0;
static idx_t  seq_count   = 0;   /* number of terms already found */
static uint64_t seq_last_prime = 0; /* last prime examined */
static bool    seq_initialized = false;

/* Ensure the cache can hold at least 'needed' terms. */
static void ensure_capacity(size_t needed)
{
    if (needed <= seq_cap)
        return;
    size_t new_cap = seq_cap ? seq_cap << 1 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_terms = seq_realloc(seq_terms, seq_cap, new_cap);
    seq_cap = new_cap;
}

/* Append a term to the cache. */
static void append_term(seq_t val)
{
    ensure_capacity(seq_count + 1);
    seq_terms[seq_count++] = val;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;  /* Offset is 1; behaviour below is irrelevant. */

    /* Initialise on first call. */
    if (!seq_initialized) {
        seq_last_prime = 2;   /* First prime to start the search. */
        seq_initialized = true;
    }

    /* Generate terms until we reach the desired index. */
    while (seq_count < n) {
        uint64_t next = next_prime(seq_last_prime);  /* next prime after seq_last_prime */

        /* Check if the next prime is exactly 18 greater. */
        if (next == seq_last_prime + 18ULL) {
            seq_t s1 = digit_sum((seq_t)seq_last_prime);
            seq_t s2 = digit_sum((seq_t)next);
            if (s1 == s2) {
                append_term((seq_t)seq_last_prime);
            }
        }
        seq_last_prime = next;
    }

    return seq_terms[n - 1];
}
