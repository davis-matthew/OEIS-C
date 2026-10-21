/*
 * A077111  a(n) = A077110(n) - n^2.
 * https://oeis.org/A077111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 7, i16 n <= 54, i32 n <= 54, i64 n <= 54, i128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A077111:  a(n) = nearest cube to n²  minus  n².
 *  For each n, let m = n².
 *  Let k = floor(cuberoot(m)).  Then
 *      low = k³,  high = (k+1)³.
 *  Let d_low = m - low, d_high = high - m.
 *  If d_low < d_high:  return -d_low;
 *  else:                return d_high.
 *  This implements the definition a(n) = A077110(n) - n².
 */

#include <stdint.h>

static seq_t floor_cuberoot(seq_t x)
{
    seq_t low  = 0;
    seq_t high = 1;

    /* find an upper bound for the cube root */
    while ((high * high * high) <= x)
    {
        high <<= 1;
        /* safety: for n up to 10⁴, x ≤ 10⁸, loop stays well below limits */
    }

    /* binary search on [low, high) */
    while (low < high)
    {
        seq_t mid = low + (high - low + 1) / 2;
        seq_t cube = mid * mid * mid;
        if (cube <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n * (seq_t)n;          /* n² in seq_t */
    seq_t k = floor_cuberoot(m);            /* floor(cuberoot(n²)) */
    seq_t low  = k * k * k;                  /* k³          */
    seq_t high = (k + 1) * (k + 1) * (k + 1);/* (k+1)³      */

    seq_t d_low  = m - low;                 /* distance below */
    seq_t d_high = high - m;                /* distance above */

    if (d_low < d_high)
        return -(d_low);
    else
        return d_high;
}
