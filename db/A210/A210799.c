/*
 * A210799  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A210800; see the Formula section.
 * https://oeis.org/A210799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the sequence of rows of the triangle T(n,k) * defined by
 *            T(1,0)=1, T(2,0)=3, T(2,1)=1 and for n≥3, *
 *            T(n,k)=T(n-1,k-1)+3·T(n-2,k)+2·T(n-2,k-1)+T(n-2,k-2)+a(k) *
 *            where a(0)=2, a(1)=-1 and a(k)=0 for k>1. * The sequence a(n) is
 *            obtained by listing the triangle row‑by‑row
 * Range:     u8 n <= 29, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mul_small.h"

static seq_t **triangle = NULL;  /* triangle rows, 1‑based index */
static size_t triangle_rows = 0; /* number of rows currently stored */

seq_t a(idx_t n)
{
    /* n is 1‑based, return 0 for n==0 (undefined) */
    if (n == 0) return (seq_t)0;

    /* Determine the row m such that sum_{i=1}^{m-1} i < n ≤ sum_{i=1}^m i */
    size_t m = 1;
    while ((m * (m + 1ULL)) / 2ULL < (size_t)n) ++m;

    /* Grow and compute rows up to m if necessary */
    if (triangle_rows < m) {
        size_t old_rows = triangle_rows;
        triangle_rows = m;
        triangle = (seq_t **)realloc(triangle,
                                     triangle_rows * sizeof(seq_t *));
        for (size_t i = old_rows; i < triangle_rows; ++i) {    /* i = row-1 */
            size_t len = i + 1;                        /* number of elements in this row */
            triangle[i] = (seq_t *)malloc(len * sizeof(seq_t));
            if (!triangle[i]) return (seq_t)0;          /* allocation failure */

            if (i == 0) {                              /* row 1: T(1,0)=1 */
                triangle[i][0] = (seq_t)1;
            } else if (i == 1) {                       /* row 2: T(2,0)=3, T(2,1)=1 */
                triangle[i][0] = (seq_t)3;
                triangle[i][1] = (seq_t)1;
            } else {                                   /* general recurrence */
                seq_t *prev1 = triangle[i - 1];      /* row n-1 */
                seq_t *prev2 = triangle[i - 2];      /* row n-2 */
                for (size_t j = 0; j <= i; ++j) {    /* k = j (0‑based) */
                    seq_t sum = 0;

                    /* T(n-1,k-1) */
                    if (j > 0) sum += prev1[j - 1];

                    /* 3·T(n-2,k)  (only if k < n-2, i.e. j < i-1) */
                    if (j < i - 1) sum += mul_small(prev2[j], 3);

                    /* 2·T(n-2,k-1)  (only if k-1 < n-2) */
                    if (j > 0 && j < i) sum += mul_small(prev2[j - 1], 2);

                    /* T(n-2,k-2)  (only if k ≥ 2) */
                    if (j >= 2) sum += prev2[j - 2];

                    /* a(k) terms */
                    if (j == 0)
                        sum += (seq_t)2;
                    else if (j == 1)
                        sum -= (seq_t)1;               /* a(1) = -1 */

                    triangle[i][j] = sum;
                }
            }
        }
    }

    /* Flatten the triangle row‑by‑row to find the n‑th element */
    size_t prev_sum = (m - 1ULL) * m / 2ULL;          /* elements before row m */
    size_t idx = (size_t)(n - prev_sum - 1ULL);       /* 0‑based index in row m */
    return triangle[m - 1][idx];
}
