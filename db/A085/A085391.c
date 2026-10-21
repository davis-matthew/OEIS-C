/*
 * A085391  Square array of centered numbers, read by antidiagonals.
 * https://oeis.org/A085391
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(i,j) with T(i,j) = C(i+j,j) - C(i,j). The indices (i,j)
 *            are obtained from n by traversing the square array T along
 *            anti‑diagonals. Let d be the smallest integer such that
 *            (d+1)(d+2)/2 > n. Then let pos = n - d(d+1)/2 // 0 ≤ pos ≤ d i =
 *            pos j = d - pos Finally a(n) = binomial(d, j) - bino...
 * Range:     u8 n <= 39, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find diagonal d: smallest d with (d+1)(d+2)/2 > n */
    idx_t d = 0;
    while ( ((d + 1) * (d + 2)) / 2 <= n )
        ++d;

    /* Position within diagonal */
    idx_t cumulativeBefore = d * (d + 1) / 2;   // elements before diagonal d
    idx_t pos = n - cumulativeBefore;           // 0 … d
    idx_t i = pos;                  // row index
    idx_t j = d - pos;              // column index

    /* Compute T(i,j) = C(d, j) - C(i, j) */
    seq_t c1 = binomial(d, j);      // C(d, j)
    seq_t c2 = binomial(i, j);      // C(i, j) (0 if j > i)
    return c1 - c2;
}
