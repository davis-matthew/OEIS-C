/*
 * A209125  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A164975; see the Formula section.
 * https://oeis.org/A209125
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th entry (row‑major) of the triangle T(n,k)
 *            defined by T(0,0) = 1, T(1,0) = 2, T(1,1) = 1, T(2,0) = 3,
 *            T(2,1) = 4, T(2,2) = 2, and for n ≥ 3: T(n,k) = T(n-1,k) +
 *            2*T(n-1,k-1) + T(n-2,k) – T(n-2,k-1), with out‑of‑range indices
 *            treated as 0. The sequence a(n) lists the triangle row‑ma...
 * Range:     u8 n <= 30, u16 n <= 57, u32 n <= 57, u64 n <= 57, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    if (n < 1) return 0;            /* undefined below offset, return 0 */

    /* find row r such that cumulative(r-1) < n ≤ cumulative(r) */
    idx_t r = 0;
    while (((uint64_t)(r + 1) * (r + 2) / 2) < n) ++r;

    /* kth element in row r (0‑based) */
    idx_t cum_prev = (uint64_t)r * (r + 1) / 2;
    idx_t k = n - cum_prev - 1;

    /* build triangle up to row r */
    seq_t *row0 = NULL, *row1 = NULL, *row2 = NULL;
    seq_t *prev2 = NULL, *prev1 = NULL, *curr = NULL;
    size_t len0 = 0, len1 = 0, len2 = 0, len_prev2 = 0, len_prev1 = 0, len_curr = 0;

    /* row 0 */
    row0 = (seq_t *)malloc(sizeof(seq_t) * 1);
    row0[0] = 1;
    len0 = 1;

    if (r == 0) {
        seq_t result = row0[0];
        free(row0);
        return result;
    }

    /* row 1 */
    row1 = (seq_t *)malloc(sizeof(seq_t) * 2);
    row1[0] = 2;
    row1[1] = 1;
    len1 = 2;

    if (r == 1) {
        seq_t result = row1[k];
        free(row0);
        free(row1);
        return result;
    }

    /* row 2 */
    row2 = (seq_t *)malloc(sizeof(seq_t) * 3);
    row2[0] = 3;
    row2[1] = 4;
    row2[2] = 2;
    len2 = 3;

    if (r == 2) {
        seq_t result = row2[k];
        free(row0);
        free(row1);
        free(row2);
        return result;
    }

    /* for r >= 3 */
    prev2 = row1;   len_prev2 = len1;   /* row r-2 */
    prev1 = row2;   len_prev1 = len2;   /* row r-1 */

    for (idx_t i = 3; i <= r; ++i) {
        curr = (seq_t *)malloc(sizeof(seq_t) * (i + 1));
        len_curr = i + 1;

        for (idx_t kk = 0; kk <= i; ++kk) {
            seq_t a = (kk < len_prev1)      ? prev1[kk]      : 0; /* T(n-1,k)   */
            seq_t b = (kk > 0 && kk-1 < len_prev1) ? prev1[kk-1] : 0; /* T(n-1,k-1) */
            seq_t c = (kk < len_prev2)      ? prev2[kk]      : 0; /* T(n-2,k)   */
            seq_t d = (kk > 0 && kk-1 < len_prev2) ? prev2[kk-1] : 0; /* T(n-2,k-1) */

            curr[kk] = a + 2 * b + c - d;
        }

        /* shift rows */
        free(prev2);          /* free row n-2 */
        prev2 = prev1;
        len_prev2 = len_prev1;
        prev1 = curr;
        len_prev1 = len_curr;
    }

    seq_t result = prev1[k];

    /* cleanup */
    free(row0);
    free(prev2);   /* now holds row r-1 */
    free(prev1);   /* holds row r */

    return result;
}
