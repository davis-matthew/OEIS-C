/*
 * A215273  Primes congruent to {0, 1, 2, 5} mod 17.
 * https://oeis.org/A215273
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the n‑th prime p such that p mod 17 ∈ {0,1,2,5}. */
#include <stdint.h>
#include <stdlib.h>
#include <stddef.h>
#include "../utils/next_prime.h"

seq_t a(idx_t n)
{
    /* static cache of qualifying primes */
    static seq_t *primes = NULL;    /* cached primes   */
    static size_t capacity = 0;     /* allocated size  */
    static idx_t count = 0;        /* number cached   */
    static uint64_t last_p = 0;    /* last prime used */

    /* Generate more primes until we have at least n of them */
    while (count < n)
    {
        uint64_t p = next_prime(last_p + 1);
        last_p = p;

        uint64_t r = p % 17U;
        if (r == 0U || r == 1U || r == 2U || r == 5U)
        {
            /* grow storage if necessary */
            if (count == capacity)
            {
                capacity = capacity == 0 ? 16 : capacity * 2;
                seq_t *tmp = (seq_t *)realloc(primes, capacity * sizeof(seq_t));
                if (!tmp)
                    abort();   /* allocation failure */
                primes = tmp;
            }
            primes[count++] = (seq_t)p;
        }
    }

    return primes[n - 1];
}
