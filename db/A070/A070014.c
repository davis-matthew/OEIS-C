/*
 * A070014  Ceiling of number of prime factors of n divided by the number of
 *          n's distinct prime factors.
 * https://oeis.org/A070014
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(bigomega(n)/omega(n))
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where omega(n)  = number of distinct prime factors of n
   and bigomega(n) = total number of prime factors of n (with multiplicity). */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* For n < 2 the offset is irrelevant; return 0. */
    if (n < 2)
        return (seq_t)0;

    idx_t m = n;
    int omega = 0;        /* distinct prime factors */
    int bigomega = 0;     /* total prime factors */

    idx_t d = 2;
    while (d * d <= m) {
        if (m % d == 0) {
            omega++;
            while (m % d == 0) {
                bigomega++;
                m /= d;
            }
        }
        d += (d == 2 ? 1 : 2);   /* 2,3,5,7,... */
    }
    if (m > 1) {              /* remaining prime factor */
        omega++;
        bigomega++;
    }

    /* Compute ceil(bigomega / omega) as (bigomega + omega - 1) / omega. */
    seq_t big = (seq_t)bigomega;
    seq_t om  = (seq_t)omega;
    return (big + om - 1) / om;
}
