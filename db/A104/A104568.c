/*
 * A104568  Triangle of numbers that are 0 or 1 mod 3.
 * https://oeis.org/A104568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   let r be the row index of n, defined by r(r+1)/2 ≤ n <
 *            (r+1)(r+2)/2; let c = n - r(r+1)/2 and m = r - c + 1. Then a(n)
 *            = m + (((r - c) & 1) ? (m + 1) / 2 : m / 2)
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   row_of_tr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/row_of_tr.h"

seq_t a(idx_t n)
{
    idx_t r = row_of_tr(n);
    __int128 tr128 = (__int128)r * (r + 1) / 2;
    idx_t tr = (idx_t)tr128;          /* triangular offset of the row */
    idx_t c = n - tr;                 /* column within the row */
    idx_t m = r - c + 1;              /* number of terms in the partial sum */
    idx_t rc = r - c;
    idx_t count_twos = (rc & 1) ? (m + 1) / 2 : m / 2;
    seq_t result = (seq_t)m + (seq_t)count_twos;
    return result;
}
