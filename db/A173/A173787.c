/*
 * A173787  Triangle read by rows: T(n,k) = 2^n - 2^k, 0 <= k <= n.
 * https://oeis.org/A173787
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 44, u16 n <= 152, u32 n <= 560, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 2^r - 2^c, where
 *   r = ⌊(√(8n + 1) – 1)/2⌋   (row index)
 *   c = n – r(r + 1)/2          (column index)
 * This reads the triangle T(n,k) = 2^n – 2^k row by row.
 */

#include <math.h>
#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* compute row index r */
    idx_t r = (idx_t)((sqrt((double)(8 * n + 1)) - 1.0) / 2.0);

    /* correct possible rounding error */
    while ((r + 1) * (r + 2) / 2 <= n) ++r;
    while (r * (r + 1) / 2 > n) --r;

    /* column index c */
    idx_t offset = r * (r + 1) / 2;
    idx_t c = n - offset;

    /* compute 2^r and 2^c and subtract */
    seq_t val_r = pow2(r);
    seq_t val_c = pow2(c);
    return val_r - val_c;   /* r >= c guarantees no underflow */
}
