/*
 * A109223  Number triangle related to the Fibonacci polynomials.
 * https://oeis.org/A109223
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(row, col) where T(n,k) = binomial(floor((n+k)/2)+k,
 *            2*k) row = row_of_index(n) = floor((sqrt(8*n+1)-1)/2) col = n -
 *            row*(row+1)/2
 * Range:     u8 n <= 70, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    // compute the row index
    long double td = sqrtl((long double)(8.0L * (long double)n + 1.0L));
    idx_t row = (idx_t)((td - 1.0L) / 2.0L);

    // compute the column index
    idx_t start = row * (row + 1) / 2;
    idx_t col   = n - start;

    // arguments for the binomial coefficient
    idx_t N = (row + col) / 2 + col;  // floor((row+col)/2)+col
    idx_t K = 2 * col;                // 2*col

    return binomial(N, K);
}
