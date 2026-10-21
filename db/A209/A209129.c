/*
 * A209129  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209128; see the Formula section.
 * https://oeis.org/A209129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   A209129 is obtained by enumerating the triangle T(n,k) * defined
 *            by T(1,0)=1, T(2,0)=0, T(2,1)=3, and *
 *            T(n,k)=T(n-1,k)+2*T(n-1,k-1)-T(n-2,k-1)+T(n-2,k-2) for 1≤k<n. *
 *            a(n) is the row‑major enumeration of T(n,k) * (a(1)=T(1,0),
 *            a(2)=T(2,0), a(3)=T(2,1), …)
 * Range:     u8 n <= 26, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                        /* index below offset, arbitrary 0 */

    /* Determine the row r such that Σ_{i=1}^{r-1} i < n ≤ Σ_{i=1}^{r} i */
    idx_t r = 1;
    idx_t sum = 1;                       /* Σ_{i=1}^{r} i */
    while (sum < n) {
        ++r;
        sum += r;
    }

    /* Position k within row r (0‑based) */
    idx_t prevSum = sum - r;             /* Σ_{i=1}^{r-1} i */
    idx_t k = n - prevSum - 1;           /* 0 ≤ k < r */

    if (r == 1)                          /* a(1) = T(1,0) = 1 */
        return (seq_t)1;

    /* Allocate temporary rows: row0 (n-2), row1 (n-1), row2 (current) */
#ifdef __cplusplus
    seq_t *row0 = new seq_t[r + 1]();
    seq_t *row1 = new seq_t[r + 1]();
    seq_t *row2 = new seq_t[r + 1]();
#else
    seq_t *row0 = (seq_t*)calloc(r + 1, sizeof(seq_t));
    seq_t *row1 = (seq_t*)calloc(r + 1, sizeof(seq_t));
    seq_t *row2 = (seq_t*)calloc(r + 1, sizeof(seq_t));
#endif

    /* Base rows */
    row0[0] = (seq_t)1;                  /* T(1,0) */
    row1[0] = (seq_t)0;                  /* T(2,0) */
    row1[1] = (seq_t)3;                  /* T(2,1) */

    if (r == 2) {                        /* a(2) or a(3) */
        seq_t result = row1[k];
#ifdef __cplusplus
        delete [] row0; delete [] row1; delete [] row2;
#else
        free(row0); free(row1); free(row2);
#endif
        return result;
    }

    /* Pointers to rotate rows: prev2 = row0, prev1 = row1, cur = row2 */
    seq_t *prev2 = row0;
    seq_t *prev1 = row1;
    seq_t *cur   = row2;

    /* Build rows 3 .. r */
    for (idx_t curRow = 3; curRow <= r; ++curRow) {
        for (idx_t kk = 0; kk < curRow; ++kk) {
            seq_t val = 0;
            /* T(n-1,k) */
            if (kk < curRow - 1)
                val += prev1[kk];
            /* 2*T(n-1,k-1) */
            if (kk > 0)
                val += (seq_t)2 * prev1[kk - 1];
            /* -T(n-2,k-1) */
            if (kk > 0)
                val -= prev2[kk - 1];
            /* +T(n-2,k-2) */
            if (kk > 1)
                val += prev2[kk - 2];
            cur[kk] = val;
        }
        /* Rotate the row buffers for the next iteration */
        seq_t *tmp = prev2;
        prev2 = prev1;
        prev1 = cur;
        cur   = tmp;
    }

    /* prev1 now points to the final row r */
    seq_t result = prev1[k];

    /* Free allocated memory */
#ifdef __cplusplus
    delete [] row0;
    delete [] row1;
    delete [] row2;
#else
    free(row0);
    free(row1);
    free(row2);
#endif

    return result;
}
