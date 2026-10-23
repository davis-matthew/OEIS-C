/*
 * A176112  Primes that take a prime number of steps to reach 1 under the
 *          Collatz function.
 * https://oeis.org/A176112
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   primes p such that A006577(p) (the number of Collatz * steps to
 *            reach 1) is also a prime. The function searches for * these
 *            primes on demand and memoises the results
 * Range:     u8 n <= 6, u16 n <= 35, u32 n <= 4418, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Static cache of found sequence elements. */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Current odd candidate for primality testing. */
    static idx_t cur = 3;           /* 2 is not in the sequence */

    /* Fill the cache until it contains at least n elements. */
    while (cache_len < n) {
        /* Find the next prime. */
        while (!is_prime(cur))
            cur += 2;        /* skip even numbers */

        /* Compute the total Collatz stopping time for this prime. */
        idx_t steps = 0;
        seq_t x = (seq_t)cur;
        while (x != (seq_t)1) {
            if ((x & (seq_t)1) == (seq_t)0)         /* even */
                x /= (seq_t)2;
            else
                x = x * (seq_t)3 + (seq_t)1;
            ++steps;
        }

        /* If the step count itself is prime, store the prime. */
        if (is_prime((idx_t)steps)) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 4;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) { /* allocation failure */
                    /* In case of critical failure, abort. */
                    abort();
                }
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)cur;
        }

        /* Advance to the next odd number for the next search. */
        cur += 2;
    }

    /* Return the requested term. */
    return cache[n - 1];
}
