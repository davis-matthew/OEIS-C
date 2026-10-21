/*
 * A217577  The only prime p such that k*a < p < k*b where a, b are
 *          consecutive primes, case k=5.
 * https://oeis.org/A217577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 231, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: For each pair of consecutive primes a and b,
 * find the unique prime p such that 5*a < p < 5*b.
 * If exactly one such prime exists, p is the next term of the sequence.
 * The function memoizes results to support random access. */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* static cache for memoization */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;
    static uint64_t last_a = 0;            /* last processed prime a */
    static bool initialized = false;

    /* initialize cache and starting prime a = 2 */
    if (!initialized) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        last_a = 2;                       /* first prime */
        initialized = true;
    }

    /* ensure we have computed up to index n (1‑based) */
    while (cache_len < (size_t)n) {
        /* grow cache if needed */
        if (cache_len == cache_cap) {
            cache_cap *= 2;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        /* next consecutive prime b */
        uint64_t b = next_prime(last_a);

        /* compute bounds 5*a < p < 5*b */
        uint64_t lo = last_a * 5ULL;
        uint64_t hi = b * 5ULL;

        /* first prime after 5*a */
        seq_t p1 = (seq_t)next_prime(lo);

        if (p1 < (seq_t)hi) {
            /* check if there is a second prime inside the interval */
            seq_t p2 = (seq_t)next_prime((uint64_t)p1);
            if (p2 >= (seq_t)hi) {
                /* unique prime found; store it */
                cache[cache_len++] = p1;
            }
        }

        /* move to the next consecutive pair */
        last_a = b;
    }

    /* return the n-th element (1‑based index) */
    return cache[(size_t)n - 1];
}
