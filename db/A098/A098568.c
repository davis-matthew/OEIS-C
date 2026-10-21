/*
 * A098568  Triangle of triangular binomial coefficients, read by rows, where
 *          column k has the g.f.: 1/(1-x)^((k+1)*(k+2)/2) for k >= 0.
 * https://oeis.org/A098568
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, changed
 * Formula:   a(n) = T(r,k) with r = row of n, k = position in row, where
 *            T(r,k) = binomial( (k+1)*(k+2)/2 + r - k - 1, r - k ). The
 *            sequence is read by rows of a triangular array
 * Range:     u8 n <= 30, u16 n <= 69, u32 n <= 160, u64 n <= 446,
 *            u128 n <= 1080, big any n
 * Helpers:   binom_custom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom_custom.h"

seq_t a(idx_t n)
{
    /* Find the row r and the index of the first element in that row. */
    idx_t r = 0;
    idx_t start = 0;                     /* index of first term of row r  */
    while (1) {
        idx_t next_start = start + (r + 1);   /* first index of next row */
        if (n < next_start)
            break;
        start = next_start;
        ++r;
    }

    /* Position within the row */
    idx_t k = n - start;                 /* 0 ≤ k ≤ r */

    /* Parameters for the binomial coefficient */
    idx_t d = r - k;                     /* r - k ≥ 0 */
    idx_t m = (k + 1) * (k + 2) / 2;     /* (k+1)(k+2)/2  */

    /* Compute T(r,k) = binomial(m + d - 1, d)  */
    return binom_custom(m + d - 1, d);
}
