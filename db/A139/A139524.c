/*
 * A139524  Triangle T(n,k) read by rows: the coefficient of [x^k] of the
 *          polynomial 2*(x+1)^n + 2^n in row n, column k.
 * https://oeis.org/A139524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy, less
 * Formula:   a(n) is the nth element of the triangle T with * T(n,0) = 2 +
 *            2^n and T(n,k) = 2 * binomial(n,k) for k>0. * The triangle is
 *            read by rows, so a(n) = T(r,c) where * r and c are determined
 *            from n by flattening the rows
 * Range:     u8 n <= 35, u16 n <= 135, u32 n <= 527, u64 n <= 1325,
 *            u128 n <= 1325, big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    idx_t row = 0;
    /* Find the row r such that (r+1)(r+2)/2 > n.
       The preceding rows contain r(r+1)/2 terms. */
    while ((row + 1) * (row + 2) / 2 <= n)
        ++row;

    idx_t terms_before = row * (row + 1) / 2;
    idx_t col = n - terms_before;

    if (col == 0) {
        seq_t two_pow = pow2(row); /* 2^row */
        return (seq_t)2 + two_pow;
    } else {
        seq_t bin = binomial(row, col);   /* binomial(row, col) */
        return (seq_t)2 * bin;
    }
}
