/*
 * A210519  a(n) = floor(volume of 4-sphere of radius n).
 * https://oeis.org/A210519
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(1/2 * PI^2 * n^4)
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 29, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>

seq_t a(idx_t n)
{
    /* high‑precision constant */
    const long double PI = acosl(-1.0L);
    /* compute n^4 as a 128‑bit integer to avoid overflow for n up to 10000 */
    unsigned __int128 n128 = (unsigned __int128)n;
    unsigned __int128 n4   = n128 * n128 * n128 * n128;

    /* evaluate the real value using long double */
    long double val = 0.5L * PI * PI * (long double)n4;

    /* apply floor (add a tiny epsilon to guard against rounding down) */
    long double floored = floorl(val + 1e-12L);

    /* cast to seq_t (value fits in 64 bits for all n considered) */
    return (seq_t)(unsigned long long)floored;
}
