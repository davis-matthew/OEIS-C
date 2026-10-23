/*
 * A115255  "Correlation triangle" of central binomial coefficients A000984.
 * https://oeis.org/A115255
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = T(i,j) where i = floor((sqrt(8*n+1)-1)/2), j = n -
 *            i*(i+1)/2 and T(i,j) = Σ_{t=0}^{min(j,i-j)} binomial(2*j-2*t,
 *            j-t) * binomial(2*i-2*j-2*t, i-j-t)
 * Range:     u8 n <= 9, u16 n <= 35, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   _floor_isqrt_u64, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* row index i from triangular number */
    idx_t root = _floor_isqrt_u64(n * 8 + 1);
    idx_t i    = (root - 1) / 2;

    /* column index j */
    idx_t row_start = (i * (i + 1)) / 2;
    idx_t j = n - row_start;

    /* compute T(i,j) */
    idx_t max_t = (j <= (i - j)) ? j : (i - j);
    seq_t sum = 0;
    for (idx_t t = 0; t <= max_t; ++t) {
        seq_t b1 = binomial(2 * j - 2 * t, j - t);
        seq_t b2 = binomial(2 * i - 2 * j - 2 * t, i - j - t);
        sum += b1 * b2;
    }
    return sum;
}
