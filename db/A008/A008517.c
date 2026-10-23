/*
 * A008517  Second-order Eulerian triangle T(n,k), 1 <= k <= n.
 * https://oeis.org/A008517
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, nice, easy, changed
 * Formula:   a(n) = T(r,k) where r is the unique integer with r*(r+1)/2 ≥ n >
 *            (r-1)*r/2, k = n - (r-1)*r/2. Recurrence for the triangle:
 *            T(1,1) = 1 T(n,k) = k*T(n-1,k) + (2*n - k)*T(n-1,k-1) for 1 ≤ k
 *            ≤ n The sequence a(n) lists the triangle entries in row‑major
 *            order
 * Range:     u8 n <= 12, u16 n <= 31, u32 n <= 62, u64 n <= 163,
 *            u128 n <= 450, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Guard against index below the offset (not required). */
    if (n == 0)
        return (seq_t)0;

    /* Find the row r and column k that correspond to index n. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    idx_t k = n - ((r - 1) * r) / 2;   /* 1 ≤ k ≤ r */

    /* Triangle rows are at most about √(2n) ≈ 150 for n ≤ 10000. */
    static seq_t prev[200];
    static seq_t cur[200];

    /* Row 1 initialization. */
    prev[1] = (seq_t)1;
    if (r == 1)
        return prev[1];

    /* Build rows 2 … r. */
    for (idx_t row = 2; row <= r; ++row) {
        cur[1] = (seq_t)1;

        for (idx_t col = 2; col <= row; ++col) {
            seq_t part1 = (seq_t)0;
            if (col <= row - 1)
                part1 = (seq_t)col * prev[col];

            seq_t part2 = (seq_t)(2 * row - col) * prev[col - 1];
            cur[col] = part1 + part2;
        }

        /* Copy current row to prev for next iteration. */
        for (idx_t j = 1; j <= row; ++j)
            prev[j] = cur[j];
    }

    return prev[k];
}
