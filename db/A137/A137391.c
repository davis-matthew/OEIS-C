/*
 * A137391  Triangle: let f(t) = 1 + t + t^2 and g(t) = t + t^2, expansion of
 *          p(t) = f(t)*exp(x*g(t)).
 * https://oeis.org/A137391
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, uned, tabl, less
 * Formula:   a(n) = nth term of the triangle obtained by expanding p(t) = (1
 *            + t + t^2) * exp(x * (t + t^2)), taking the coefficient of
 *            t^(r-1) and the coefficient of x^k in that coefficient,
 *            multiplied by (r-1)! ; then all elements of the resulting row r
 *            are listed row‑wise
 * Range:     u8 n <= 23, u16 n <= 49, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    /* Convert to 1‑based index of the triangle entry */
    idx_t idx = n;            /* n is already 1‑based */
    /* Find zero‑based row number r and column number k.
       r is the row index (0‑based), k is the column index (0‑based). */
    idx_t r = 0;
    while (((r + 1) * (r + 2)) / 2 < idx) {
        ++r;
    }
    idx_t prev_sum = r * (r + 1) / 2;   /* elements before row r */
    idx_t k = idx - prev_sum - 1;       /* column index in row r */

    if (r == 0) {        /* row 0 : only element 1 */
        return 1;
    }

    /* Prepare rows for DP */
    /* row 0 */
    seq_t *prev2 = (seq_t*)malloc(sizeof(seq_t) * 1);
    prev2[0] = 1;

    if (r == 1) {        /* row 1 : {1, 1} */
        seq_t res = (k == 0) ? 1 : 1;
        free(prev2);
        return res;
    }

    /* row 1 */
    seq_t *prev1 = (seq_t*)malloc(sizeof(seq_t) * 2);
    prev1[0] = 1;
    prev1[1] = 1;

    /* Build rows up to r */
    for (idx_t i = 2; i <= r; ++i) {
        seq_t *cur = (seq_t*)malloc(sizeof(seq_t) * (i + 1));

        /* c_{i,0} */
        if (i == 2) {
            cur[0] = 2;   /* 2! * coeff of t^2 in f(t) = 2 */
        } else {
            cur[0] = 0;   /* coeff of t^i in f(t) is 0 for i > 2 */
        }

        /* For k = 1 .. i */
        for (idx_t col = 1; col <= i; ++col) {
            /* term1 = prev1[col-1] * i / col */
            seq_t term1 = prev1[col - 1] * (seq_t)i / (seq_t)col;
            seq_t term2 = 0;
            if (col < i) {   /* prev2 has size i (i-1 + 1) */
                term2 = prev2[col - 1] * (seq_t)i * (seq_t)(i - 1) / (seq_t)col;
            }
            cur[col] = term1 + term2;
        }

        /* Rotate rows */
        free(prev2);
        prev2 = prev1;
        prev1 = cur;
    }

    /* prev1 now holds row r */
    seq_t result = prev1[k];
    free(prev2);
    free(prev1);
    return result;
}
