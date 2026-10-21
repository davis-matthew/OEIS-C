/*
 * A098833  Numbers n such that the sum of primes dividing n (with repetition)
 *          is a Fibonacci number.
 * https://oeis.org/A098833
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 12, u32 n <= 127, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   factor_u64, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A098833: numbers n such that the sum of prime factors of n (with
   multiplicity) is a Fibonacci number.  The function searches
   successive integers, factors each using trial division, sums the
   prime factors, and tests whether the sum is a Fibonacci number
   using the classic 5·x²±4 criterion.  Results are cached so that
   subsequent calls are efficient even if the indices are out of
   order. */
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/factor_u64.h"      /* factor_u64 */
#include "../utils/floor_sqrt.h"     /* floor_sqrt */

/* Return true if x is a Fibonacci number by testing 5x²±4. */
static bool is_fibonacci(seq_t x)
{
    seq_t five = (seq_t)5;
    seq_t test1 = five * x * x + (seq_t)4;
    seq_t s1 = floor_sqrt(test1);
    if (s1 * s1 == test1) {
        return true;
    }
    seq_t test2 = five * x * x - (seq_t)4;
    seq_t s2 = floor_sqrt(test2);
    return (s2 * s2 == test2);
}

seq_t a(idx_t n)
{
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    /* current integer that has been fully examined */
    static uint64_t tested = 0;

    /* Grow the cache if necessary */
    while (cache_len < n) {
        uint64_t candidate = ++tested;

        /* Sum of prime factors with multiplicity */
        seq_t sum = (seq_t)0;
        if (candidate != 1) {
            /* Factor candidate.  64 primes are enough for any 64‑bit number. */
            uint64_t primes[64];
            uint64_t exps[64];
            size_t cnt;
            factor_u64(candidate, primes, exps, &cnt);
            for (size_t i = 0; i < cnt; ++i) {
                seq_t p = (seq_t)primes[i];
                seq_t e = (seq_t)exps[i];
                /* sum += p * e */
                sum += p * e;
            }
        } /* else sum remains 0 for candidate == 1 */

        if (is_fibonacci(sum)) {
            /* Ensure capacity for the cache */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)candidate;
        }
    }

    return cache[n - 1];
}
