/*
 * A091362  Primes p such that the sum of the digits of p is not prime, but
 *          the sum of the squares of the digits of p is prime.
 * https://oeis.org/A091362
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 199, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_square_sum, digit_sum, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A091362: primes p such that digit_sum(p) is not prime,
   and digit_square_sum(p) is prime.  The sequence is built by
   testing successive integers for primality and the two digit
   conditions, caching found terms for future calls. */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>

/* Required utilities from the build */
#include "../utils/digit_sum.h"          /* seq_t digit_sum(seq_t v);   */
#include "../utils/digit_square_sum.h"   /* uint64_t digit_square_sum(uint64_t n); */
#include "../utils/is_prime_small.h"     /* bool is_prime_small(uint64_t n); */

static inline void grow_array(void **ptr, size_t *cap, size_t needed, size_t elem)
{
    if (*cap < needed) {
        size_t new_cap = (*cap == 0 ? 8 : *cap * 2);
        while (new_cap < needed) new_cap <<= 1;
        void *tmp = realloc(*ptr, new_cap * elem);
        if (!tmp) abort(); /* out of memory */
        *ptr = tmp;
        *cap = new_cap;
    }
}

/* Cache of sequence values */
static seq_t  *term_cache     = NULL;
static size_t  term_cache_len = 0;
static size_t  term_cache_cap = 0;

/* Next integer to test; persists between calls */
static uint64_t next_candidate = 2;

/* Append a new term to the cache */
static void append_term(seq_t val)
{
    grow_array((void **)&term_cache, &term_cache_cap,
               term_cache_len + 1, sizeof(seq_t));
    term_cache[term_cache_len++] = val;
}

seq_t a(idx_t n)
{
    /* Generate terms until we have at least n of them */
    while (term_cache_len < (size_t)n) {
        uint64_t cand = next_candidate;

        /* Advance to the next integer */
        next_candidate = cand + 1;

        /* Skip even numbers greater than 2 */
        if (cand > 2 && (cand & 1U) == 0)
            continue;

        /* Primality test (fast for 64‑bit numbers) */
        if (!is_prime_small(cand))
            continue;

        /* Digit sum and digit‑square sum checks */
        uint64_t dsum  = (uint64_t)digit_sum((seq_t)cand);
        uint64_t dssq  = digit_square_sum(cand);

        if (!is_prime_small(dsum) && is_prime_small(dssq)) {
            append_term((seq_t)cand);
        }
    }

    /* Return the n-th term (1‑based indexing) */
    return term_cache[n - 1];
}
