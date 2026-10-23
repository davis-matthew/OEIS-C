/*
 * A157180  A new general triangle sequence based on the Eulerian form in
 *          three parts ( subtraction):m=2; t0(n,k)=If[n*k == 0, 1, Sum[(-1)^j
 *          Binomial[n + 1, j](k + 1 - j)^n, {j, 0, k + 1}]] t(n,k,m)=If[n ==
 *          0, 1, ( m*(n - k) + 1)*t0(n - 1 + 1, k - 1) + (m*k + 1)*t0(n - 1 +
 *          1, k) - m*k*(n - k)*t0(n - 2 + 1, k...
 * https://oeis.org/A157180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the nth entry (row‑by‑row) of triangle T(n,k,2) defined
 *            by m = 2; t0(n,k) = 1 if n*k==0 t0(n,k) = Σ_{j=0}^{k+1} (-1)^j
 *            C(n+1,j) (k+1-j)^n t(n,k,m) = 1 if n==0 t(n,k,m) = (m*(n-k)+1) *
 *            t0(n-1,k-1) + (m*k+1) * t0(n-1,k) - m*k*(n-k) * t0(n-2,k-1) The
 *            function returns the flattened sequence a...
 * Range:     u8 n <= 16, u16 n <= 38, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

/*--------------------------------------------------------------------
  Compute the triangle entry t(n,k,2) where n is the row index and k the
  column index (both 0‑based).  The row index n therefore corresponds
  to the vertical index 'r' in the flattened sequence.
--------------------------------------------------------------------*/
static seq_t triangle_entry(idx_t r, idx_t k)
{
    /* ------------------------------------------------------------
       Build t0 rows up to row r.
       t0[n][k] is stored as a 1‑D array for each row.
    ------------------------------------------------------------ */
    if (r == 0) return (seq_t)1;   /* t(0,0)=1 */

    /* row 0 */
    seq_t *prev = (seq_t*)malloc(sizeof(seq_t));
    if (!prev) return (seq_t)0;
    prev[0] = (seq_t)1;          /* t0(0,0)=1 */

    /* build rows 1 .. r-1 into 'prev' (row r-1) */
    for (idx_t nrow = 1; nrow < r; ++nrow)
    {
        seq_t *curr = (seq_t*)malloc(sizeof(seq_t)*(nrow+1));
        if (!curr) { free(prev); return (seq_t)0; }

        curr[0] = (seq_t)1;          /* t0(nrow,0)=1 */
        curr[nrow] = (seq_t)0;       /* t0(nrow,nrow)=0 */

        for (idx_t j = 1; j < nrow; ++j)
        {
            /* t0(nrow,j) = (j+1)*t0(nrow-1,j) + (nrow-j)*t0(nrow-1,j-1) */
            seq_t term1 = (seq_t)(j + 1) * prev[j];
            seq_t term2 = (seq_t)(nrow - j) * prev[j - 1];
            curr[j] = term1 + term2;
        }

        free(prev);
        prev = curr;   /* next iteration uses this as previous row */
    }

    /* ------------------------------------------------------------
       Compute row r into 'curr' (row r).  After this, 'prev' holds
       row r-1 and 'curr' holds row r.
    ------------------------------------------------------------ */
    seq_t *curr = (seq_t*)malloc(sizeof(seq_t)*(r+1));
    if (!curr) { free(prev); return (seq_t)0; }

    curr[0] = (seq_t)1;
    curr[r] = (seq_t)0;

    for (idx_t j = 1; j < r; ++j)
    {
        seq_t term1 = (seq_t)(j + 1) * prev[j];
        seq_t term2 = (seq_t)(r - j) * prev[j - 1];
        curr[j] = term1 + term2;
    }

    /* ------------------------------------------------------------
       Evaluate t(r,k,2) from the definition.
    ------------------------------------------------------------ */
    const seq_t m = 2;
    seq_t result = (seq_t)0;

    if (k > 0) {
        seq_t coeff = (seq_t)(m * (r - k) + 1);
        result += coeff * curr[k - 1];
    }
    seq_t coeff2 = (seq_t)(m * k + 1);
    result += coeff2 * curr[k];

    if (k > 0) {
        seq_t coeff3 = (seq_t)(m * k * (r - k));
        result -= coeff3 * prev[k - 1];
    }

    free(prev);
    free(curr);
    return result;
}

/*--------------------------------------------------------------------
   Main sequence function: converts flattened index n to (row, col)
   and returns the triangle entry.
--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    /* find row r such that n lies in row 0..r */
    idx_t r = 0;
    idx_t offset = 0;          /* first index of current row */
    while (n >= offset + r + 1) {
        offset += r + 1;
        ++r;
    }
    idx_t k = n - offset;      /* column in that row */

    return triangle_entry(r, k);
}
