/*
 * A062993  A triangle (lower triangular matrix) composed of Pfaff-Fuss (or
 *          Raney) sequences.
 * https://oeis.org/A062993
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) is obtained by scanning the lower‑triangular matrix a(r,k)
 *            where a(r,k) = binomial((k+2)*(r-k), r-k) / ((k+1)*(r-k)+1). The
 *            flattening is row‑wise: a(n) = a(row, col) with row =
 *            floor((sqrt(8n+1)-1)/2), col = n - row*(row+1)/2
 * Range:     u8 n <= 9, u16 n <= 28, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find r with r*(r+1)/2 <= n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }

    /* Column index within the row. */
    idx_t col = n - r * (r + 1) / 2;

    /* Compute the triangle entry a(r, col). */
    idx_t m   = r - col;          /* r - k */
    idx_t k2  = col + 2;          /* k + 2 */
    seq_t num = binomial(k2 * m, m);
    seq_t den = (seq_t)(col + 1) * m + 1;

    return num / den;   /* exact division */
}
