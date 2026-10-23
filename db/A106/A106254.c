/*
 * A106254  Partition table in square format.
 * https://oeis.org/A106254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = number of partitions of i with at most j parts, where *
 *            the pair (i,j) is obtained from the antidiagonal mapping of the
 *            square * partition table
 * Range:     u8 n <= 305, u16 n <= 406, u32 n <= 406, u64 n <= 406,
 *            u128 n <= 406, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

 /* Helper to compute s*(s-1)/2 safely using 128‑bit arithmetic. */
static inline idx_t triangular(idx_t s)
{
    unsigned __int128 t = (unsigned __int128)s * (s - 1) / 2;
    return (idx_t)t;
}

/* Maximum dimension needed for n ≤ 10000 (√(2n) < 150). Use a buffer
 * that is comfortably large but still small for static allocation.       */
#define MAXS 250
static seq_t dp[MAXS][MAXS];

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset is 1; undefined for n==0 */

    /* Find the diagonal (sum s) that contains the n‑th entry. */
    idx_t low = 1, high = 1;
    while (triangular(high) < n)
        high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if (triangular(mid) >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t s = low;

    idx_t totalprev = triangular(s - 1);
    idx_t pos = n - totalprev;      /* 1‑based position within the diagonal */

    /* Map to table indices (row i, column j). */
    idx_t i = s - pos;              /* row   (1‑based)   */
    idx_t j = pos;                  /* column(1‑based)   */

    if (i >= MAXS || j >= MAXS) return 0;  /* safety guard */

    /* Compute T(i,j) = partitions of i with at most j parts. */
    /* Base row n = 0: one empty partition for every m. */
    for (idx_t m = 0; m <= j; ++m)
        dp[0][m] = 1;

    /* Compute the table row by row. */
    for (idx_t nrow = 1; nrow <= i; ++nrow) {
        dp[nrow][0] = 0;                      /* no partitions of positive n with 0 parts */
        for (idx_t mcol = 1; mcol <= j; ++mcol) {
            seq_t val = dp[nrow][mcol - 1];
            if (nrow >= mcol)
                val += dp[nrow - mcol][mcol];
            dp[nrow][mcol] = val;
        }
    }

    return dp[i][j];
}
