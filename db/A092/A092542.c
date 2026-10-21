/*
 * A092542  Table whose n-th row is constant and equal to n, read by
 *          antidiagonals alternately upwards and downwards.
 * https://oeis.org/A092542
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, frac, nonn, tabl
 * Formula:   a(n) = ((-1)^t+1)*j/2 - ((-1)^t-1)*i/2, where i = n - t*(t+1)/2,
 *            j = (t*t + 3*t + 4)/2 - n, t = floor((sqrt(8*n-7)-1)/2). The
 *            implementation finds t by binary search without using floating
 *            point
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   calc_t
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/calc_t.h"

seq_t a(idx_t n)
{
    if (n == 0)     /* offset is 1; behaviour for n < 1 is unused. */
        return 0;

    idx_t t = calc_t(n);

    /* i = n - t*(t+1)/2 */
    unsigned __int128 tri = (__int128)t * (t + 1) / 2;
    idx_t i = n - (idx_t)tri;

    /* j = (t*t + 3*t + 4)/2 - n */
    unsigned __int128 tt = t;
    unsigned __int128 j_term = tt * tt + 3 * tt + 4;
    idx_t j = (idx_t)(j_term / 2) - n;

    /* If t is even, return j, otherwise return i. */
    return (t & 1) ? (seq_t)i : (seq_t)j;
}
