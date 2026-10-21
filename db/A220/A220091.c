/*
 * A220091  Number of ways to write n=p+q+(n mod 2)q with p>q and p, q, 6q-1,
 *          6q+1 all prime.
 * https://oeis.org/A220091
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of pairs (p,q) of primes such that p > q, p, q,
 *            6*q-1, 6*q+1 are all prime, and n = p + q if n is even n = p +
 *            2*q if n is odd (i.e. n = p + q + (n mod 2)*q)
 * Range:     u8 n <= 99, u16 n <= 9431, u32 n <= 9431, u64 n <= 9431,
 *            u128 n <= 9431, big any n
 * Helpers:   isPrimeGeneric
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/isPrimeGeneric.h"

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    bool odd = ((n & 1) != 0);

    /* upper bound for q:
       if n even: 2*q < n   => q <= (n-1)/2
       if n odd : 3*q < n   => q <= (n-1)/3
       use the safe bound for all cases: q <= n/2
    */
    idx_t qmax = n / 2;

    for (idx_t q = 2; q <= qmax; ++q)
    {
        if (!isPrimeGeneric(q))
            continue;

        seq_t qseq = q;
        seq_t sixq = qseq * 6;

        /* check twin primes 6q-1 and 6q+1 */
        if (!isPrimeGeneric(sixq - 1))
            continue;
        if (!isPrimeGeneric(sixq + 1))
            continue;

        seq_t p;
        if (odd)
        {
            /* n = p + 2q --> p = n - 2q */
            if (n <= 2 * q)   /* p would be <= 0 or <=q */
                continue;
            p = (seq_t)n - ((seq_t)(q) * 2);
        }
        else
        {
            /* n = p + q --> p = n - q */
            if (n <= q)
                continue;
            p = (seq_t)n - ((seq_t)q);
        }

        /* p must be > q and prime */
        if (p <= qseq)
            continue;
        if (!isPrimeGeneric(p))
            continue;

        ++cnt;
    }
    return cnt;
}
