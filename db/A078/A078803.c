/*
 * A078803  Triangular array T given by T(n,k) = number of compositions of n
 *          into k parts, each in the set {1,2,3}.
 * https://oeis.org/A078803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where r is the row such that r(r+1)/2 ≥ n * and k
 *            = n - r(r-1)/2. T(r,k) counts the compositions of r into k *
 *            parts each of size 1, 2, or 3, and satisfies * T(r,k) =
 *            T(r-1,k-1) + T(r-2,k-1) + T(r-3,k-1), with T(0,0)=1
 * Range:     u8 n <= 72, u16 n <= 243, u32 n <= 843, u64 n <= 3047,
 *            u128 n <= 9954, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>   /* for malloc, free */

seq_t a(idx_t n)
{
    if (n == 0)          /* offset starts at 1, return 0 for n==0 */
        return 0;

    /* Find the row r: smallest r with r(r+1)/2 ≥ n */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;

    /* Column k within that row (1‑based) */
    idx_t k = n - r * (r - 1) / 2;

    /* Allocate a square table (r+1)×(r+1) for DP, using 1‑based
     * indices for convenience.  The table is stored in row‑major order.
     */
    size_t dim = (size_t)(r + 1);
    size_t sz   = dim * dim;
    seq_t *dp   = (seq_t *)malloc(sz * sizeof(seq_t));
    if (!dp)                 /* allocation failed – return 0 as fallback */
        return 0;

    /* Initialize all entries to 0 */
    for (size_t i = 0; i < sz; ++i)
        dp[i] = 0;

    /* Base: T(0,0) = 1 */
    dp[0] = 1;

    /* Fill the DP table */
    for (idx_t i = 1; i <= r; ++i) {
        seq_t *row = dp + i * dim;
        for (idx_t j = 1; j <= i; ++j) {
            seq_t val = 0;
            if (i >= 1 && j >= 1)
                val += dp[(i - 1) * dim + (j - 1)];
            if (i >= 2 && j >= 1)
                val += dp[(i - 2) * dim + (j - 1)];
            if (i >= 3 && j >= 1)
                val += dp[(i - 3) * dim + (j - 1)];
            row[j] = val;
        }
    }

    /* Result is T(r,k) */
    seq_t result = dp[r * dim + k];

    free(dp);
    return result;
}
