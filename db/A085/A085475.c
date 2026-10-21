/*
 * A085475  Square array read by antidiagonals: T(n,k) = binomial(2*n+k, k) -
 *          binomial(2*n, k).
 * https://oeis.org/A085475
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(row, col) where row + col = d, n = d(d+1)/2 + row and
 *            T(i, j) = binomial(2*i + j, j) - binomial(2*i, j)
 * Range:     u8 n <= 22, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find diagonal d so that d(d+1)/2 <= n < (d+1)(d+2)/2   */
    idx_t d = 0;
    idx_t start_d = 0;
    while (1)
    {
        idx_t next_start = (d + 1) * (d + 2) / 2;
        if (next_start <= n)
        {
            ++d;
            start_d = next_start;
        }
        else
        {
            start_d = d * (d + 1) / 2;
            break;
        }
    }
    /* row is the offset within the diagonal, col is remaining part */
    idx_t row = n - start_d;
    idx_t col = d - row;

    /* Compute T(row, col) = binom(2*row + col, col) - binom(2*row, col) */
    seq_t val1 = binom(row * 2 + col, col);
    seq_t val2 = binom(row * 2, col);
    return val1 - val2;
}
