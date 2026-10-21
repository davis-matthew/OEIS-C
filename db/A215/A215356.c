/*
 * A215356  Primes congruent to {0, 2, 3, 4, 5} mod 17.
 * https://oeis.org/A215356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is the n-th prime p with p ≡ 0, 2, 3, 4, or 5 (mod 17)
 * Range:     u8 n <= 17, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function grows a cache of such primes as needed.           */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime.h"

/* Public sequence function ----------------------------------------------------- */
seq_t a(idx_t n)
{
    /* Offset is 1 – behaviour for n==0 is irrelevant. */
    if (n == 0) return 0;

    /* Static cache of qualifying primes and the next number to test. */
    static seq_t *qprimes = NULL;       /* array of qualifying primes */
    static size_t qcnt   = 0;           /* numbers stored so far */
    static size_t qcap   = 0;           /* current capacity of qprimes   */
    static idx_t  next_candidate = 2;   /* next integer to test for primality */

    /* Keep filling the cache until it contains at least n primes. */
    while (qcnt < n)
    {
        if (is_prime(next_candidate))
        {
            idx_t r = next_candidate % 17;
            if (r == 0 || r == 2 || r == 3 || r == 4 || r == 5)
            {
                /* Expand capacity if necessary. */
                if (qcnt >= qcap)
                {
                    qcap = qcap ? qcap * 2 : 16;
                    seq_t *tmp = (seq_t *)realloc(qprimes, qcap * sizeof(seq_t));
                    if (!tmp) exit(EXIT_FAILURE);
                    qprimes = tmp;
                }
                /* Store the qualifying prime. */
                qprimes[qcnt++] = (seq_t)next_candidate;
            }
        }
        ++next_candidate;
    }

    return qprimes[n - 1];
}
