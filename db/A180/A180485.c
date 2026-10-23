/*
 * A180485  Lucas multiplication table as a triangle read by rows T(n, k) =
 *          L(n)L(k), with L(n) being a Lucas number (A000032).
 * https://oeis.org/A180485
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = L(r) * L(k), where r is the least positive integer with
 *            r*(r+1)/2 ≥ n, and k = n - r*(r-1)/2 (the column index, 1‑based)
 * Range:     u8 n <= 20, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   lucas_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/lucas_u64.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 >= n */
    idx_t r = 1;
    while (((__int128)r * (r + 1)) / 2 < n) {
        ++r;
    }

    /* Previous triangular number: (r-1)*r/2 */
    idx_t prev_tri = (((__int128)(r - 1) * r) / 2);

    /* Column index k (1‑based) */
    idx_t k = n - prev_tri;  /* because prev_tri < n <= prev_tri + r */

    /* Compute Lucas numbers for row and column */
    seq_t L_r = lucas_u64(r);
    seq_t L_k = lucas_u64(k);

    return L_r * L_k;
}
