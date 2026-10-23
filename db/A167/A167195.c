/*
 * A167195  a(2)=3, for n>=3, a(n)=a(n-1)+gcd(n, a(n-1)).
 * https://oeis.org/A167195
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(2) = 3, for n>=3, a(n) = a(n-1) + gcd(n, a(n-1))
 * Range:     u8 n <= 153, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/gcd.h"

seq_t a(idx_t n)
{
    /* Behaviour for indices below the OEIS offset (n < 2) is unspecified.
       Returning 0 is harmless and keeps the function well‑defined. */
    if (n < 2)
        return (seq_t)0;

    /* The first term of the sequence. */
    seq_t val = (seq_t)3;  /* a(2) */

    if (n == 2)
        return val;

    /* Compute terms up to n. */
    for (idx_t i = 3; i <= n; ++i)
    {
        seq_t g = gcd(val, (seq_t)i);
        val += g;
    }

    return val;
}
