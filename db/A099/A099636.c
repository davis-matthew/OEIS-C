/*
 * A099636  a(n) = gcd(sum of distinct prime factors of n, product of distinct
 *          prime factors of n).
 * https://oeis.org/A099636
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd( sum of distinct prime factors of n, product of
 *            distinct prime factors of n )
 * Range:     u8 n <= 256, u16 n <= 8920, u32 n <= 8920, u64 n <= 8920,
 *            u128 n <= 8920, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes A099636.  Uses simple trial‑division factorisation
   (fast enough for n up to 10 000 in the expected tests) */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Handle the trivial case n == 1 */
    if (n == 1)
        return (seq_t)1;

    uint64_t m = (uint64_t)n;
    seq_t prod = (seq_t)1;
    seq_t sum  = (seq_t)0;

    /* Trial division */
    for (uint64_t p = 2; p * p <= m; ++p)
    {
        if (m % p == 0)
        {
            prod = prod * (seq_t)p;
            sum  = sum + (seq_t)p;
            while (m % p == 0)
                m /= p;
        }
    }

    /* m now is a prime > sqrt(original n) (or 1) */
    if (m > 1)
    {
        prod = prod * (seq_t)m;
        sum  = sum + (seq_t)m;
    }

    return gcd(prod, sum);
}
