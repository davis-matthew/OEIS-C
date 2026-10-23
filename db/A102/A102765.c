/*
 * A102765  Array read by antidiagonals: T(n, k) = ((n+4)^k-(n-1)^k)/5.
 * https://oeis.org/A102765
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = Read the 2‑D array T(n,k) = ((n+4)^k-(n-1)^k)/5 along
 *            antidiagonals (row index decreases within each diagonal)
 * Range:     u8 n <= 18, u16 n <= 42, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   uint_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/uint_pow.h"

seq_t a(idx_t n)
{
    /* Find diagonal d such that d*(d+1)/2 <= n < (d+1)*(d+2)/2. */
    idx_t d = 0;
    while (((( __int128)(d + 1) * (d + 2)) / 2) <= n)
        d++;

    idx_t tri = (idx_t)((( __int128)d * (d + 1)) / 2);  /* t_d = d*(d+1)/2 */
    idx_t offset = n - tri;           /* index within the diagonal (k) */
    idx_t col    = offset;            /* column index k   */
    idx_t row    = d - offset;        /* row index (n)    */

    /* Compute T(row, col) = ((row+4)^col - (row-1)^col)/5. */
    if (row == 0)                /* n = 0 case: handle sign of (n-1)^k */
    {
        seq_t pow4 = uint_pow((seq_t)4, col);
        seq_t h;
        if (col & 1)  /* k odd: (-1)^k = -1 */
            h = pow4 + 1;
        else
            h = pow4 - 1;
        return h / 5;
    }
    else
    {
        seq_t upper = uint_pow((seq_t)(row + 4), col);
        seq_t lower = uint_pow((seq_t)(row - 1), col);
        return (upper - lower) / 5;
    }
}
