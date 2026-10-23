/*
 * A127717  Triangle read by rows. T(n, k) = k * binomial(n + 1, k + 1), for 1
 *          <= k <= n.
 * https://oeis.org/A127717
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = k * binomial(r+1, k+1) . * r = the row number (1‑based)
 *            containing the n‑th element, i.e. the * smallest r with r(r+1)/2
 *            ≥ n. * k = column index within that row (1‑based). The value in
 *            the * triangle is T(r,k) = k·binomial(r+1, k+1)
 * Range:     u8 n <= 7, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binomial, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 1) return 0;                     /* undefined for n < 1 */

    /* --- find the row r containing index n --- */
    seq_t disc = (seq_t)8 * (seq_t)n + 1;     /* 8*n + 1 */
    seq_t sq   = floor_sqrt(disc);            /* floor(sqrt(disc)) */
    idx_t sqrt_floor = (idx_t)sq;             /* fits into idx_t (r ≤ ~141 for n≤10000) */

    idx_t r_floor = (sqrt_floor - 1) / 2;     /* floor((sqrt(disc)-1)/2) */
    idx_t tri_floor = r_floor * (r_floor + 1) / 2;  /* triangular number of r_floor */

    idx_t r = (tri_floor < n) ? r_floor + 1 : r_floor; /* ceil to the correct row */

    /* --- column index k within that row --- */
    idx_t row_start = r * (r - 1) / 2 + 1;    /* first linear index of this row */
    idx_t offset = n - row_start;             /* 0‑based offset in the row */
    idx_t k = offset + 1;                     /* 1‑based column index */

    /* --- compute T(r,k) = k * binomial(r+1, k+1) --- */
    seq_t bin = binomial(r + 1, k + 1);       /* binomial argument fits in idx_t */
    return (seq_t)k * bin;
}
