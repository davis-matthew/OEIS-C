/*
 * A200259  Numbers k such that k-th prime has an even digit sum.
 * https://oeis.org/A200259
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 26, u16 n <= 3181, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   build_primes, digit_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Numbers k such that the k-th prime has an even digit sum.
   Computation: iterate through primes, count those with an even digit sum,
   and return the index when the count reaches n. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/build_primes.h"
#include "../utils/digit_sum.h"

seq_t a(idx_t n)
{
    /* Persistent storage of primes across calls. */
    static idx_t *primes = NULL;
    static size_t prime_count = 0;
    static idx_t prime_limit = 300000;

    while (1)
    {
        /* Build prime table if not yet built. */
        if (prime_count == 0)
        {
            build_primes(prime_limit, &primes, &prime_count);
            if (prime_count == 0) return 0;   /* safeguard */
        }

        idx_t even_cnt = 0;
        for (idx_t i = 0; i < prime_count; ++i)
        {
            seq_t s = digit_sum((seq_t)primes[i]);   /* digit sum of the prime */
            if ((s & 1) == 0)                      /* even digit sum? */
            {
                ++even_cnt;
                if (even_cnt == n)                 /* n-th such k found */
                    return (seq_t)(i + 1);         /* k is 1‑based index */
            }
        }

        /* Not enough primes found yet; extend the limit and rebuild. */
        prime_limit *= 2;
        if (primes) {
            free(primes);
            primes = NULL;
        }
        prime_count = 0;
    }
}
