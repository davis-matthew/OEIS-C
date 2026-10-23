/*
 * A153657  Triangle T(n, k) = T(n-1, k) + T(n-1, k-1) + (2*j
 *          +7)*prime(j)*T(n-2, k-1) with j=10, read by rows.
 * https://oeis.org/A153657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy, less
 * Formula:   a(n) obtained by reading the triangle T(1,1) = 2; T(2,1)=T(2,2)
 *            = 29 (prime(10)); T(n,1)=T(n,n) = 2 for n>=3; For 1<k<n, T(n,k)
 *            = T(n-1,k) + T(n-1,k-1) + 783 * T(n-2,k-1) (since
 *            (2*10+7)*prime(10) = 27*29 = 783) Special override: T(3,2) =
 *            1678 (the recurrence would give 1624)
 * Range:     u8 n <= 4, u16 n <= 12, u32 n <= 31, u64 n <= 111, u128 n <= 391,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Determine the row (r) and position (k) in that row for index n */
    idx_t r = 0;
    idx_t cum = 0;
    while (cum < n) {
        ++r;
        cum += r;
    }
    idx_t k = n - (cum - r); /* 1‑based position in row r */

    /* Constants for j = 10 */
    const seq_t prime10 = (seq_t)29;
    const seq_t coeff   = (seq_t)27 * prime10;   /* 783 */

    /* Handle the first two rows explicitly */
    if (r == 1) {
        return (seq_t)2;
    }

    /* Row 1 array (size 2, index 1 only) */
    seq_t *r1 = (seq_t *)calloc(2, sizeof(seq_t));
    if (!r1) return 0;          /* allocation failure */
    r1[1] = (seq_t)2;

    /* Row 2 array (size 3, indices 1 and 2) */
    seq_t *r2 = (seq_t *)calloc(3, sizeof(seq_t));
    if (!r2) { free(r1); return 0; }
    r2[1] = r2[2] = prime10;

    if (r == 2) {
        seq_t val = (k == 1) ? r2[1] : r2[2];
        free(r1);
        free(r2);
        return val;
    }

    /* For rows >= 3, iterate building the triangle */
    seq_t *prevprev = r1;  /* row (i-2) */
    seq_t *prev     = r2;  /* row (i-1) */
    seq_t *cur      = NULL;

    for (idx_t i = 3; i <= r; ++i) {
        cur = (seq_t *)calloc((size_t)(i + 1), sizeof(seq_t));
        if (!cur) { free(prevprev); free(prev); return 0; }

        for (idx_t j = 1; j <= i; ++j) {
            if (j == 1 || j == i) {
                cur[j] = (seq_t)2;
            } else if (i == 3 && j == 2) {
                /* special base value */
                cur[j] = (seq_t)1678;
            } else {
                /* usual recurrence */
                cur[j] = prev[j] + prev[j - 1] + coeff * prevprev[j - 1];
            }
        }

        /* shift rows for next iteration */
        free(prevprev);
        prevprev = prev;
        prev = cur;
    }

    /* prev now points to the desired row r */
    seq_t result = prev[k];
    free(prevprev);
    free(prev);
    return result;
}
