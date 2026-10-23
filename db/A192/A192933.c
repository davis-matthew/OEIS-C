/*
 * A192933  Triangle read by rows: T(n,k) = Sum_{i <= n, j <= k, (i,j) <>
 *          (n,k)} T(i,j), starting with T(1,1) = 1, for n >= 1 and 1 <= k <=
 *          n.
 * https://oeis.org/A192933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is produced by the triangle T(n,k) with T(1,1)=1 and for
 *            n>=1, 1<=k<=n : T(n,k) = Σ_{i<=n, j<=k, (i,j)!=(n,k)} T(i,j).
 *            The sequence a(n) reads this triangle row by row
 * Range:     u8 n <= 13, u16 n <= 33, u32 n <= 77, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* Find row R and column C such that n = (R-1)R/2 + C, 1 <= C <= R. */
    idx_t R = 1;
    seq_t tri = (seq_t)1;          /* (R-1)R/2 for current R.         */
    while (tri < n) {
        ++R;
        tri = (seq_t)R * ((seq_t)R + 1) / 2;
    }
    seq_t prevTri = (seq_t)(R - 1) * (seq_t)R / 2;
    idx_t C = (idx_t)(n - prevTri);   /* 1‑based column index.      */

    /* Allocate array for S(n-1,k) values; size R+2 to allow access
       for k = R+1 (full sum of previous rows). */
    size_t sz = (size_t)R + 2;
    seq_t *Sprev = (seq_t *)calloc(sz, sizeof(seq_t));
    if (!Sprev) return (seq_t)0;

    Sprev[1] = (seq_t)1;     /* S(1,1) = 1. */
    Sprev[2] = (seq_t)1;     /* For k>=2, S(1,k) = 1 (full sum). */

    seq_t answer = (seq_t)0;

    /* Build rows sequentially until row R is reached. */
    for (idx_t cur = 2; cur <= R; ++cur) {
        size_t curSize = (size_t)cur + 2;
        seq_t *Snew = (seq_t *)calloc(curSize, sizeof(seq_t));
        if (!Snew) {
            free(Sprev);
            return (seq_t)0;
        }

        seq_t rowPrefix = (seq_t)0;
        for (idx_t k = 1; k <= cur; ++k) {
            /* T(cur,k) = S(cur-1,k) + sum_{j<k} T(cur,j) = Sprev[k] + rowPrefix. */
            seq_t val = Sprev[k] + rowPrefix;
            rowPrefix += val;

            if (cur == R && k == C)
                answer = val;

            /* S(cur,k) = S(cur-1,k) + sum_{j<=k} T(cur,j) = Sprev[k] + rowPrefix. */
            Snew[k] = Sprev[k] + rowPrefix;
        }
        /* For k > cur, S(cur,k) equals the full sum of rows 1..cur. */
        Snew[cur + 1] = Snew[cur];

        free(Sprev);
        Sprev = Snew;
    }

    free(Sprev);
    return answer;
}
