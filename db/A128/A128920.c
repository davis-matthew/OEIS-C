/*
 * A128920  Sum of all the factors in all the ways to write n as n = x*y*z
 *          with 1 <= x <= y <= z <= n.
 * https://oeis.org/A128920
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{x*y*z = n, 1 <= x <= y <= z} (x + y + z)
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */

seq_t a(idx_t n)
{
    if (n == 0) return 0;                /* safety, though n>=1 in the sequence */

    seq_t sum = 0;

    /* Enumerate possible x such that x divides n */
    for (idx_t x = 1; x <= n; ++x)
    {
        if (n % x != 0) continue;        /* skip non‑divisors */

        idx_t m = n / x;                 /* remaining factor product y*z */

        /* Enumerate y such that x <= y and y divides m */
        /* y must not exceed sqrt(m), because if y>sqrt(m) then z<m/y<y, violating y <= z */
        for (idx_t y = x; y * y <= m; ++y)
        {
            if (m % y != 0) continue;    /* y is not a divisor of m */

            idx_t z = m / y;             /* z = m / y = n / (x*y) */
            /* Ensure ordering constraint y <= z (guaranteed by y <= sqrt(m)) */
            if (z < y) continue;         /* safety check */

            /* Add the sum of the triple to the total */
            sum += (seq_t)x + (seq_t)y + (seq_t)z;
        }
    }

    return sum;
}
