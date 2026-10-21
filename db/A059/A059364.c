/*
 * A059364  Triangle T(n,k)=Sum_{i=0..n} |stirling1(n,n-i)|*binomial(i,k),
 *          k=0..n-1.
 * https://oeis.org/A059364
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the nth entry of the triangle T(n,k), n≥1, defined by *
 *            T(1,0)=1, T(n,k)=0 for k<0 or k≥n, and for n>1 * T(n,k) =
 *            (n-1)*T(n-1,k-1) + n*T(n-1,k). * The sequence a(n) lists the
 *            rows of this triangle in order
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 68, u64 n <= 159,
 *            u128 n <= 443, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Sequence entry function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset starts at 1 */

    /* Find row r such that (r‑1)*r/2 < n ≤ r*(r+1)/2 */
    idx_t cum = 0;
    idx_t r   = 1;
    while (cum + r < n) {
        cum += r;
        ++r;
    }
    idx_t k = n - cum - 1;          /* 0‑based column in row r */

    /* Static memoised triangle */
    static seq_t **triangle = NULL; /* triangle[rr][kk] 1‑based row index, 0‑based column */
    static idx_t rows = 0;          /* number of rows currently computed */

    /* Grow the triangle up to row r if necessary */
    if (rows < r) {
        /* Allocate or grow the array of row pointers */
        size_t new_rows = r + 1;            /* keep index 0 unused for clarity */
        triangle = (seq_t **)realloc(triangle, new_rows * sizeof(seq_t *));
        for (idx_t rr = rows + 1; rr <= r; ++rr) {
            /* Allocate storage for row rr (rr elements) */
            triangle[rr] = (seq_t *)malloc(sizeof(seq_t) * rr);

            for (idx_t kk = 0; kk < rr; ++kk) {
                seq_t left  = (kk > 0)          ? triangle[rr-1][kk-1] : 0;
                seq_t right = (kk < rr-1) ? triangle[rr-1][kk]     : 0;

                if (rr == 1) {
                    /* Base case: T(1,0) = 1 */
                    triangle[rr][kk] = (kk == 0) ? (seq_t)1 : (seq_t)0;
                } else {
                    seq_t part1 = (seq_t)((seq_t)(rr-1) * left);
                    seq_t part2 = (seq_t)((seq_t)rr * right);
                    triangle[rr][kk] = part1 + part2;
                }
            }
        }
        rows = r;
    }

    return triangle[r][k];
}
