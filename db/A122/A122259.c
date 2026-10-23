/*
 * A122259  Primes p such that p - 1 is not 3-smooth.
 * https://oeis.org/A122259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 41, u16 n <= 6511, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function computes OEIS sequence A122259
 * (primes p such that p−1 is not 3‑smooth) by searching primes
 * in increasing order and caching the results.  It works for
 * any seq_t/idx_t configuration (uint64_t, __int128, arbitrary precision).
 * The cache guarantees that calls may arrive in any order.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Cached terms of the sequence */
    static seq_t *cache = NULL;
    static size_t cache_sz = 0;   /* number of terms stored */
    static size_t cache_cap = 0;  /* capacity of cache array   */

    /* Next integer to test for primality */
    static idx_t next = 2;

    /* Handle calls with n==0 safely (offset is 1) */
    if (n == 0)
        return 0;

    /* Extend the cache until it contains the required n‑th term */
    while (cache_sz < n) {
        /* find the next prime   */
        do {
            next++;
        } while (next < 2 || !is_prime(next));

        /* Check whether (p-1) has a prime factor >3.
         * For any seq_t implementation this works because we
         * use only integer arithmetic operators that are defined
         * for both built‑in and arbitrary‑precision types.     */
        seq_t m = (seq_t)next - 1;          /* p - 1          */
        while ((m % 2) == 0)                /* remove all 2s   */
            m = m / 2;
        while ((m % 3) == 0)                /* remove all 3s   */
            m = m / 3;

        if (m > 1) {                        /* remainder > 1 → condition satisfied */
            /* Expand cache if necessary */
            if (cache_sz == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp)                      /* allocation failed */
                    return 0;
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_sz++] = (seq_t)next;     /* store the term   */
        }
    }

    return cache[n - 1];                      /* 1‑based indexing */
}
