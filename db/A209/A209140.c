/*
 * A209140  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209139; see the Formula section.
 * https://oeis.org/A209140
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th entry (row major) of the integer triangle
 *            T(n,k) defined by: T(1,0) = 1, T(2,0) = 1, T(2,1) = 3, T(n,k) =
 *            T(n-1,k) + 2*T(n-1,k-1) + T(n-2,k) + T(n-2,k-2) for n ≥ 3, k ≥
 *            0, k < n, T(n,k)=0 for k<0 or k≥n. The sequence (a(1),a(2),…) is
 *            obtained by walking the triangle row by row an...
 * Range:     u8 n <= 23, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define MAXR 200   /* sufficient for n ≤ 10000 (r ≈ 141) */

seq_t a(idx_t n)
{
    /* Find the row r such that the first r(r+1)/2 terms cover n. */
    idx_t r = 1;
    while ((r * (r + 1)) / 2 < n)
        ++r;

    /* Position of n within row r (0‑based). */
    idx_t prev_sum = ((r - 1) * r) / 2;
    idx_t idx_in_row = n - prev_sum - 1;

    /* Small static arrays to hold two previous rows and the current row. */
    seq_t prev2[MAXR] = {0};   /* row n−2  */
    seq_t prev1[MAXR] = {0};   /* row n−1  */
    seq_t curr[MAXR]  = {0};

    /* Base rows (n=1 and n=2). */
    prev2[0] = (seq_t)1;      /* T(1,0) */
    prev1[0] = (seq_t)1;      /* T(2,0) */
    prev1[1] = (seq_t)3;      /* T(2,1) */

    /* Direct answers for the first two rows. */
    if (r == 1)
        return prev2[0];
    if (r == 2)
        return prev1[idx_in_row];

    /* Build successive rows up to r. */
    for (idx_t rr = 3; rr <= r; ++rr) {
        for (idx_t k = 0; k < rr; ++k) {
            seq_t val = 0;

            /* T(n-1,k) */
            if (k < rr - 1)
                val += prev1[k];

            /* 2·T(n-1,k-1) */
            if (k > 0)
                val += 2 * prev1[k - 1];

            /* T(n-2,k) */
            if (k < rr - 2)
                val += prev2[k];

            /* T(n-2,k-2) */
            if (k > 1)
                val += prev2[k - 2];

            curr[k] = val;
        }

        /* Shift rows for next iteration. */
        for (idx_t i = 0; i < rr - 1; ++i)
            prev2[i] = prev1[i];
        for (idx_t i = 0; i < rr; ++i)
            prev1[i] = curr[i];
    }

    return prev1[idx_in_row];
}
