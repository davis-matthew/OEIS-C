/*
 * A100898  Triangle read by rows: T(n,k) is the number of k-matchings of the
 *          fan graph on n+1 vertices (i.e., the join of the path graph on n
 *          vertices with one extra vertex).
 * https://oeis.org/A100898
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) is the flattened sequence of the triangle of k‑matchings of
 *            the fan graph F_{r+1} (join of a path on r vertices with one
 *            extra vertex). For a given flattened index n we find the row r
 *            and the column k such that 0 ≤ k ≤ ⌈r/2⌉: T(r,k) = C(r-k,k) +
 *            Σ_{i=1}^{r} Σ_{t=0}^{k-1} C((i-1)-t , t) · C((...
 * Range:     u8 n <= 43, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   binom_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_small.h"

seq_t a(idx_t n)
{
    /* Locate the row r and the column k in the flattened triangle. */
    idx_t row = 0;
    idx_t cum = 0;
    for (;;) {
        /* Length of row r: 1 + ceil(r/2) */
        idx_t len = ((row + 1) / 2) + 1;
        if (n < cum + len)
            break;
        cum += len;
        ++row;
    }
    idx_t k = n - cum;          /* column index, 0 ≤ k ≤ ceil(row/2) */

    /* Base term: matchings that avoid the central vertex */
    seq_t res = binom_small(row - k, k);

    /* Add matchings that use exactly one edge incident to the center. */
    if (k > 0) {
        idx_t max_t = k - 1;
        for (idx_t i = 1; i <= row; ++i) {
            idx_t mL = i - 1;          /* vertices left of i */
            idx_t mR = row - i;        /* vertices right of i */
            for (idx_t t = 0; t <= max_t; ++t) {
                /* Ensure enough vertices for t left edges and
                   max_t-t right edges.  The conditions below
                   guarantee nCr is defined. */
                if (mL >= 2 * t && mR >= 2 * (max_t - t)) {
                    /* left part: (mL - t) choose t */
                    seq_t left  = binom_small(mL - t, t);
                    /* right part: (mR - (max_t-t)) choose (max_t-t) */
                    seq_t right = binom_small(mR - (max_t - t), max_t - t);
                    res += left * right;
                }
            }
        }
    }

    return res;
}
