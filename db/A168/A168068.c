/*
 * A168068  Array T(n,k) read by antidiagonals: T(n,2k+1) = 2k+1. T(n,2k) =
 *          2^n*k.
 * https://oeis.org/A168068
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = if ((j & 1) == 0) 2^i * (j/2) else j, where i,j are
 *            obtained from n by the antidiagonal order: let d =
 *            floor((sqrt(1+8n)-1)/2), offset = n - d(d+1)/2, then i = d -
 *            offset, j = offset
 * Range:     u8 n <= 56, u16 n <= 96, u32 n <= 96, u64 n <= 96, u128 n <= 96,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* find diagonal index d such that d(d+1)/2 <= n < (d+1)(d+2)/2 */
    idx_t low = 0, high = 1;
    __int128 tri;
    while ((tri = (__int128)high * (high + 1) / 2) <= n) high <<= 1;
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if ((__int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    idx_t d = low;

    /* compute offset inside the diagonal */
    idx_t tri_low = (idx_t)(((__int128)d * (d + 1)) / 2);
    idx_t offset   = n - tri_low;

    idx_t i = d - offset;   /* row */
    idx_t j = offset;       /* column */

    /* compute T(i,j) */
    if (j & 1ULL) {                          /* odd column */
        return (seq_t)j;
    } else {                                 /* even column */
        seq_t p2 = pow2(i);                  /* 2^i */
        return p2 * (seq_t)(j >> 1);        /* (j/2) * 2^i */
    }
}
