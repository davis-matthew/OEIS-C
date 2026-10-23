/*
 * A194038  Natural interspersion of A034856, a rectangular array, by
 *          antidiagonals.
 * https://oeis.org/A194038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   For a given n, let d be the unique integer such that d*(d+1)/2 ≤
 *            n < (d+1)*(d+2)/2. Let t = n - d*(d+1)/2, i = t, j = d - t. Then
 *            a(n) = T(i,j) where base = (d+1)(d+2)/2 // triangular(d+1) shift
 *            = j if i == 0 = -1 if i == 1 = -(j+3) if i ≥ 2 a(n) = base +
 *            shift
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* find diagonal d */
    idx_t d = 0;
    while ( ((unsigned __int128)(d + 1) * (d + 2)) / 2 <= n )
        d++;

    /* offset inside diagonal */
    idx_t tri_prev = ((unsigned __int128)d * (d + 1)) / 2;
    idx_t t = n - tri_prev;
    idx_t i = t;               /* row index */
    idx_t j = d - t;           /* column index */

    /* base = triangular(d+1) */
    unsigned __int128 base128 = ((unsigned __int128)(d + 1) * (d + 2)) / 2;

    /* compute signed shift */
    int64_t shift;
    if (i == 0) {
        shift = (int64_t)j;
    } else if (i == 1) {
        shift = -1;
    } else {
        shift = -( (int64_t)j + 3 );
    }

    /* sum base and shift using 128‑bit math, then cast to seq_t */
    __int128 val = (__int128)base128 + shift;
    return (seq_t)val;
}
