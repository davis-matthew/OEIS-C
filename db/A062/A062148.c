/*
 * A062148  Second (unsigned) column sequence of triangle A062138 (generalized
 *          a=5 Laguerre).
 * https://oeis.org/A062148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+1)! * binomial(n+6, 6) = ((n+1)*(n+6)!)/720
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 15, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Compute product of 2..(n+6) once, cancel 720 during multiplication,
       then multiply an extra (n+1) (the factorial includes (n+1) once,
       the binomial numerator includes (n+1)...(n+6)). */
    seq_t res = 1;
    /* primes needed to cancel 720 = 2^4 * 3^2 * 5 */
    int need2 = 4;
    int need3 = 2;
    int need5 = 1;

    /* multiply factors 2 .. n+6, cancelling primes as we go */
    for (idx_t i = 2; i <= n + 6; ++i)
    {
        idx_t val = i;
        if (need2)
        {
            while (need2 && (val % 2 == 0))
            {
                val /= 2;
                --need2;
            }
        }
        if (need3)
        {
            while (need3 && (val % 3 == 0))
            {
                val /= 3;
                --need3;
            }
        }
        if (need5)
        {
            while (need5 && (val % 5 == 0))
            {
                val /= 5;
                --need5;
            }
        }
        res *= (seq_t)val;
    }

    /* multiply the extra (n+1) from the factorial part */
    seq_t extra = (seq_t)(n + 1);
    if (need2)
    {
        while (need2 && (n + 1) % 2 == 0)
        {
            extra /= 2;
            --need2;
        }
    }
    if (need3)
    {
        while (need3 && (n + 1) % 3 == 0)
        {
            extra /= 3;
            --need3;
        }
    }
    if (need5)
    {
        while (need5 && (n + 1) % 5 == 0)
        {
            extra /= 5;
            --need5;
        }
    }
    res *= extra;

    /* All cancellations should have been used */
    return res;
}
