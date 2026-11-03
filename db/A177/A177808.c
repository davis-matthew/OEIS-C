/*
 * A177808  Triangle T(n,m) = binomial(4*n, 4*m), 0 <= m <= n, read by rows.
 * https://oeis.org/A177808
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binomial(4*row, 4*col) where row is the unique * integer
 *            satisfying row*(row+1)/2 <= n < (row+1)*(row+2)/2 * and col = n
 *            - row*(row+1)/2 (i.e. reading the triangle * T(r,c) =
 *            binomial(4*r,4*c) by rows)
 * Range:     u8 n <= 6, u16 n <= 16, u32 n <= 48, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 55 terms through n = 54 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n) {
        ++r;
    }

    /* Column index within the row */
    idx_t col = n - r * (r + 1) / 2;

    /* Compute binomial(4*r, 4*col) */
    return binomial(4 * r, 4 * col);
}
