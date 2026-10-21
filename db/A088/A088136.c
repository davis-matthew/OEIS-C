/*
 * A088136  Primes such that sum of first and last digits is prime.
 * https://oeis.org/A088136
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 22, u16 n <= 2587, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for primes whose first + last decimal digits form a prime. */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    static seq_t *cached = NULL;  /* cached qualifying primes */
    static size_t len   = 0;      /* number of cached primes   */
    static size_t cap   = 0;      /* allocated capacity        */
    static idx_t  cur   = 2;      /* next candidate to test   */

    while (len < n)
    {
        if (is_prime(cur))
        {
            /* extract first and last decimal digits of cur */
            idx_t last  = cur % 10;
            idx_t first = cur;
            while (first >= 10)
                first /= 10;

            /* check if the sum of the digits is prime */
            idx_t sum = first + last;
            if (    sum == 2 || sum == 3 || sum == 5 || sum == 7
                  || sum == 11 || sum == 13 || sum == 17)
            {
                if (len == cap)
                {
                    cap = cap ? cap * 2 : 16;
                    cached = (seq_t *)realloc(cached, cap * sizeof(seq_t));
                }
                cached[len++] = (seq_t)cur;
            }
        }

        /* move to next odd candidate (skip even numbers after 2) */
        if (cur == 2)
            cur = 3;
        else
            cur += 2;
    }

    return cached[n - 1];
}
