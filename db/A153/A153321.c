/*
 * A153321  Primes p such that abs(p^2 - 60) and p^2 + 60 are also primes.
 * https://oeis.org/A153321
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the nth prime p such that abs(p²-60) and p²+60 are both
 *            prime
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 11, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"          /* bool is_prime(idx_t) */
#include "../utils/is_prime_big.h"      /* bool is_prime_big(seq_t) */

static seq_t *cache      = NULL;   /* stored sequence values */
static size_t cache_len  = 0;      /* number of values generated */
static size_t cache_cap  = 0;      /* allocated capacity */
static idx_t  next_candidate = 2;  /* next integer to test for primality */

/* Ensure the cache has at least 'needed' slots. */
static void ensure_cache_cap(size_t needed)
{
    if (cache_cap < needed) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 8;
        if (new_cap < needed) new_cap = needed;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failed – abort to avoid undefined behaviour */
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }
}

/* Generate terms until cache_len reaches at least n. */
static void compute_next_terms(idx_t n)
{
    /* Loop until we have the required number of terms */
    while (cache_len < n) {
        /* Find the next candidate prime */
        for (idx_t p = next_candidate; ; ++p) {
            next_candidate = p + 1;          /* advance for next call */
            if (!is_prime(p)) continue;      /* skip composites */

            /* Compute p² in full precision */
            seq_t p2 = (seq_t)p * p;

            /* Compute |p²-60| and p²+60 */
            seq_t diff = (p2 > 60) ? (p2 - 60) : (60 - p2);
            seq_t sum  = p2 + 60;

            /* Test primality of the two values */
            if (is_prime_big(diff) && is_prime_big(sum)) {
                /* Store the prime p as a seq_t value */
                ensure_cache_cap(cache_len + 1);
                cache[cache_len++] = (seq_t)p;
                break;                    /* proceed to next term */
            }
        }
    }
}

/* External linkage – required entry point */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for n=0 */
    compute_next_terms(n);
    return cache[n - 1];                  /* sequence is 1‑based */
}
