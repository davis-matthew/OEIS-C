/*
 * A059473  Triangle T(n, k) is coefficient of z^n*w^k in 1/(1 - 2*z - 2*w -
 *          2*z*w) read by rows in order 00, 10, 01, 20, 11, 02, ...
 * https://oeis.org/A059473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(n',k') where (n',k') runs over the triangle
 *            coefficients * of 1/(1-2z-2w-2zw) in antidiagonal (decreasing‑n)
 *            order
 * Range:     u8 n <= 16, u16 n <= 47, u32 n <= 143, u64 n <= 539,
 *            u128 n <= 1274, big any n
 * Helpers:   binomial, pow2, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow2.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n)
{
    /* Find the antidiagonal containing the n-th term (0‑based). */
    idx_t N = n + 1;                    /* 1‑based position in the sequence */
    idx_t d = 0;
    while (((d + 1) * (d + 2)) / 2 < N)
        ++d;                            /* first d with cumulative >= N */

    idx_t prev    = d * (d + 1) / 2;     /* terms before diagonal d   */
    idx_t offset  = N - prev - 1;       /* position inside diagonal d */
    idx_t row     = d - offset;         /* n'  (row index)           */
    idx_t col     = offset;             /* k'  (column index)        */

    /* Compute T(row,col) = Σ_{j=0..row} 2^{row+col-j}·C(row,j)·C(row+col-j,row) */
    seq_t sum = 0;
    for (idx_t j = 0; j <= row; ++j) {
        seq_t t = pow2(row + col - j);
        t = seq_mul(t, binomial(row, j));
        t = seq_mul(t, binomial(row + col - j, row));
        sum += t;
    }
    return sum;
}
