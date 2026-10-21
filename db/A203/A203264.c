/*
 * A203264  Permanent of the n-th principal submatrix of (A134446 in square
 *          format).
 * https://oeis.org/A203264
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ∏_{k=1..n} k^2 * ⌊(k+1)/2⌋
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t res = 1;
    for (idx_t k = 1; k <= n; ++k) {
        seq_t kk   = (seq_t)k;           /* k as seq_t   */
        seq_t kk2  = kk * kk;            /* k^2          */
        seq_t half = ((seq_t)k + 1) / 2; /* floor((k+1)/2) */
        res *= kk2 * half;               /* multiply step */
    }
    return res;
}
