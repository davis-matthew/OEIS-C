/*
 * A131110  A000012 * A133084.
 * https://oeis.org/A131110
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = binom(r+1, j+1) if j is odd, binom(r, j) if j is even,
 *            where r = smallest integer with r(r+1)/2 ≥ n (row number), j = n
 *            - r(r-1)/2 (position in that row, 1‑based)
 * Range:     u8 n <= 23, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r(r+1)/2 ≥ n */
    idx_t low = 1, high = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t tri = (mid * (mid + 1)) / 2;
        if (tri >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t r = low;

    /* Position j within row r (1‑based) */
    idx_t prev_tot = ((r - 1) * r) / 2;
    idx_t j = n - prev_tot;

    /* Compute the required binomial coefficient */
    if (j & 1)
        return binom(r + 1, j + 1);
    else
        return binom(r, j);
}
