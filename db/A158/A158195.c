/*
 * A158195  Prime numbers such that sum of digits is even semiprime.
 * https://oeis.org/A158195
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, less
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_sum, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A158195: primes whose digit sum is an even semiprime */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/digit_sum.h"

/* static cache for the sequence terms */
static seq_t *cache   = NULL;
static idx_t cache_cnt = 0;   /* number of cached terms */
static idx_t cache_cap = 0;   /* allocated capacity of cache */
static uint64_t last_candidate = 2;   /* last number tested for primality */

/* helper to test whether a candidate integer is a "special prime" */
static bool is_special_prime(uint64_t candidate)
{
    /* not prime? */
    if (!is_prime(candidate)) return false;
    /* sum of digits */
    seq_t sum = digit_sum((seq_t)candidate);
    uint64_t s = (uint64_t)sum;
    /* must be even */
    if ((s & 1u) != 0u) return false;
    /* half must itself be prime */
    return is_prime(s >> 1);
}

/* public entry point for the sequence */
seq_t a(idx_t n)
{
    if (n < 1u) return 0;                     /* undefined for n<1 */

    /* grow cache if necessary */
    while (cache_cnt < n)
    {
        /* next odd candidate */
        if (last_candidate <= 2u) last_candidate = 3u;
        else                      last_candidate += 2u;

        if (is_special_prime(last_candidate))
        {
            /* ensure there is space in the cache */
            if (cache_cnt >= cache_cap)
            {
                cache_cap = (cache_cap == 0u) ? 8u : cache_cap * 2u;
                seq_t *new_cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                if (!new_cache) abort();    /* allocation failed */
                cache = new_cache;
            }
            cache[cache_cnt++] = (seq_t)last_candidate;
        }
    }
    return cache[n - 1u];
}
