/*
 * A069138  Triangle formed by multiplying Stirling numbers of 2nd kind
 *          S2(n,m) (A008277) by m+1.
 * https://oeis.org/A069138
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = (m+1) * S2(r,m) where (r,m) is the row and column of the
 *            first‑increasing triangle T(n,m) = (m+1) * S2(n,m) that OEIS
 *            A069138 enumerates. S2(n,m) is the Stirling number of the second
 *            kind
 * Range:     u8 n <= 17, u16 n <= 48, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function builds S2 matrix lazily up to the row
   required by the argument n. The matrix is stored
   in a static 2‑D array of seq_t values.  */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1, but guard against 0 */

    /* Find the row r such that r(r-1)/2 < n <= r(r+1)/2.
       For n <= 10000, r is at most about 150, so a linear search
       is inexpensive even when repeated many times. */
    idx_t r = 1;
    while ((seq_t)r * (r + 1) / 2 < n) ++r;

    /* Number of entries before row r */
    idx_t prev_sum = (r - 1) * r / 2;
    idx_t m = n - prev_sum;        /* 1 <= m <= r */

    /* Lazy construction of the Stirling‑2 table */
    static seq_t **s2 = NULL;      /* 0‑indexed rows */
    static idx_t maxRow = 0;       /* highest row computed so far */

    /* Initialize s2[0][0] = 1 if not done yet */
    if (maxRow == 0) {
        s2 = (seq_t **)malloc(sizeof(seq_t *));
        s2[0] = (seq_t *)malloc(sizeof(seq_t));
        s2[0][0] = 1;
        maxRow = 0;
    }

    /* Extend the table up to row r if necessary */
    if (r > maxRow) {
        seq_t **new_s2 = (seq_t **)realloc(s2, (r + 1) * sizeof(seq_t *));
        /* Compute new rows */
        for (idx_t i = maxRow + 1; i <= r; ++i) {
            new_s2[i] = (seq_t *)malloc((i + 1) * sizeof(seq_t));
            new_s2[i][0] = 0;          /* S2(i,0)=0 for i>0 */
            for (idx_t k = 1; k <= i; ++k) {
                seq_t term1 = (i - 1 >= k) ? (seq_t)k * new_s2[i - 1][k] : 0;
                seq_t term2 = (i - 1 >= k - 1) ? new_s2[i - 1][k - 1] : 0;
                new_s2[i][k] = term1 + term2;
            }
        }
        s2 = new_s2;
        maxRow = r;
    }

    /* Compute the triangle entry T(r,m) = (m+1) * S2(r,m) */
    seq_t val = (seq_t)(m + 1) * s2[r][m];
    return val;
}
