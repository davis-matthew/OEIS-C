/*
 * A158283  Prime numbers p such that 1 = abs(final digit of p - sum of all
 *          the other digits of p).
 * https://oeis.org/A158283
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, less
 * Formula:   a(n) = prime p such that |(last digit of p) - (sum of the other
 *            digits of p)| = 1
 * Range:     u8 n <= 9, u16 n <= 202, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   absdiff_int, digit_sum, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/digit_sum.h"
#include "../utils/absdiff_int.h"

/* Cached values of the sequence. */
static seq_t *cache   = NULL;   /* array of primes satisfying the property */
static size_t cache_len = 0;    /* current number of cached terms */
static size_t cache_cap = 0;    /* allocated capacity of the cache */
static idx_t   next_candidate = 2;  /* next integer to test for primality */

seq_t a(idx_t n)
{
    if (n < 1)          /* undefined for offset < 1 */
        return 0;

    /* Lazily create the cache on first use. */
    if (!cache) {
        cache_cap = 16;
        cache_len = 0;
        cache     = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
    }

    /* Extend the cache until it contains at least n terms. */
    while (cache_len < n) {
        /* Skip non‑primes. */
        while (!is_prime(next_candidate))
            ++next_candidate;

        seq_t prime = (seq_t)next_candidate;

        int last_digit   = (int)(next_candidate % 10);
        seq_t sum_all    = digit_sum(prime);
        int  sum_other   = (int)sum_all - last_digit;

        if (absdiff_int(last_digit, sum_other) == 1) {
            /* Grow the cache if necessary. */
            if (cache_len >= cache_cap) {
                cache_cap <<= 1;
                cache = (seq_t *)realloc(cache, sizeof(seq_t) * cache_cap);
            }
            cache[cache_len++] = prime;
        }

        ++next_candidate;
    }

    return cache[n - 1];
}
