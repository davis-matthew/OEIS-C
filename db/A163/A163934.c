/*
 * A163934  Triangle related to the asymptotic expansion of E(x,m=4,n).
 * https://oeis.org/A163934
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = binom(m+2,3) * stirling1(n+2,m+2) where (n,m) are the row
 *            and column (1‑based) of the linear index n in the triangle. The
 *            unsigned Stirling numbers of the first kind are used, which
 *            eliminates the sign factor seen in the OEIS definition
 * Range:     u8 n <= 7, u16 n <= 21, u32 n <= 56, u64 n <= 172, u128 n <= 498,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Includes */
#include <stdlib.h>
#include "../utils/binom.h"

/* Table for unsigned Stirling numbers of the first kind (c).
   c[n][k] stores c(n,k) for 0 ≤ k ≤ n. */
static seq_t **c_table = NULL;
static idx_t c_maxN = 0;

/* Ensure that c_table contains rows up to nMax (inclusive). */
static void ensure_stirling(idx_t nMax)
{
    if (nMax <= c_maxN)
        return;

    size_t new_rows = (size_t)nMax + 1;
    c_table = (seq_t **)realloc(c_table, new_rows * sizeof(seq_t *));
    if (!c_table) exit(1);

    /* Initialize row 0 if this is the first expansion. */
    if (c_maxN == 0) {
        c_table[0] = (seq_t *)malloc(sizeof(seq_t));
        if (!c_table[0]) exit(1);
        c_table[0][0] = 1;
    }

    /* Build successive rows. */
    for (idx_t n = c_maxN + 1; n <= nMax; ++n) {
        c_table[n] = (seq_t *)malloc((n + 1) * sizeof(seq_t));
        if (!c_table[n]) exit(1);
        for (idx_t k = 0; k <= n; ++k) {
            seq_t val;
            if (k == 0) {
                val = 0;
            } else if (k == n) {
                val = 1;
            } else {
                /* c(n,k) = c(n-1,k-1) + (n-1)*c(n-1,k) */
                val = c_table[n - 1][k - 1] + c_table[n - 1][k] * (n - 1);
            }
            c_table[n][k] = val;
        }
    }

    c_maxN = nMax;
}

/* Primary function to compute a(n). */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;

    /* Find the row r such that r(r+1)/2 >= n. */
    idx_t r  = 1;
    idx_t tri = 1;                /* r*(r+1)/2 for r = 1 */
    while (tri < n) {
        ++r;
        tri += r;
    }

    idx_t t_prev = tri - r;        /* (r-1)*r/2 */
    idx_t m = n - t_prev;          /* column number, 1‑based, 1 ≤ m ≤ r */

    /* Compute Stirling1(r+2, m+2) and binomial C(m+2,3). */
    idx_t N = r + 2;
    idx_t K = m + 2;

    ensure_stirling(N);

    seq_t stir = c_table[N][K];                /* unsigned Stirling number */
    seq_t bin  = binom(m + 2, 3);              /* binomial coefficient */

    return bin * stir;
}
