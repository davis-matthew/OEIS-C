/*
 * A225678  Triangle read by rows, T(n,k) = sum_{j=0..n} (-1)^(n+k+j)
 *          A(n,j)*C(j,n-k), A(n,j) the Eulerian numbers; n >= 0, k >= 0.
 * https://oeis.org/A225678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 24, i16 n <= 49, i32 n <= 57, i64 n <= 57, i128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*-----------------------------------------------------------------------
 *  OEIS A225678 – Triangle read by rows
 *  a(n) = T(⌊(√(8n+1)−1)/2⌋ , n − r(r+1)/2)
 *
 *  T(r,k) = ∑_{j=0..r} (−1)^{r+k+j} * A(r,j) * C(j,r−k)
 *  where A(r,j) are Eulerian numbers and C is the binomial coefficient.
 *
 *---------------------------------------------------------------------*/
#if defined(__cplusplus)
#  include <cstddef>
#  include <cstdint>
#  include <cstdlib>
#else
#  include <stddef.h>
#  include <stdint.h>
#  include <stdlib.h>
#endif

/*-----  binomial coefficient C(n,k)  ---------------------------------*/
static seq_t binom_int(idx_t n, idx_t k)
{
    if (k > n) return 0;
    if (k == 0 || k == n) return 1;
    if (k > n - k) k = n - k;              /* use symmetry */
    seq_t res = 1;
    for (idx_t i = 1; i <= k; ++i) {
        /* multiply then divide – division is exact */
        res = (res * (seq_t)(n - k + i)) / (seq_t)i;
    }
    return res;
}

/*-----  compute the Eulerian row A(r,0..r)  --------------------------*/
static seq_t *eulerian_row(idx_t r)
{
    seq_t *prev = (seq_t*)malloc((r + 1) * sizeof(seq_t));
    seq_t *cur  = (seq_t*)malloc((r + 1) * sizeof(seq_t));
    if (!prev || !cur) return NULL;          /* allocation failed */

    prev[0] = 1;                              /* A(0,0) = 1 */

    for (idx_t i = 1; i <= r; ++i) {
        for (idx_t j = 0; j <= i; ++j) {
            seq_t t1 = 0, t2 = 0;
            if (j > 0)  t1 = prev[j - 1] * (seq_t)(i - j);
            if (j < i)  t2 = prev[j] * (seq_t)(j + 1);
            cur[j] = t1 + t2;
        }
        /* swap buffers for next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur  = tmp;
    }

    free(cur);                               /* free the old buffer */
    return prev;                             /* (prev) holds the row r */
}

/*-----  triangle entry T(r,k)  --------------------------------------*/
static seq_t triangle_T(idx_t r, idx_t k)
{
    if (k > r) return 0;                     /* safety check */

    seq_t *row = eulerian_row(r);
    if (!row) return 0;                      /* allocation error */

    seq_t total = 0;
    idx_t min_j = r - k;                     /* j must be ≥ r−k */

    for (idx_t j = min_j; j <= r; ++j) {
        seq_t term = row[j] * binom_int(j, r - k);
        if (((r + k + j) & 1U) != 0) term = -term;
        total += term;
    }

    free(row);
    return total;
}

/*-----  public OEIS function a(n)  ----------------------------------*/
seq_t a(idx_t n)
{
    /* Determine row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while (((r + 1U) * (r + 2U)) / 2U <= n) ++r;

    idx_t row_start = r * (r + 1) / 2;
    idx_t k = n - row_start;                 /* column within the row */

    return triangle_T(r, k);
}
