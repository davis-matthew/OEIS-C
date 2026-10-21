/*
 * A230000  Triangular array read by rows: row n shows the coefficients of the
 *          polynomial u(n) = c(0) + c(1)*x + ... + c(k)*x^k which is the
 *          numerator of the n-th convergent of the continued fraction [1,
 *          1/x, 1/x^2, ... ,1/x^n].
 * https://oeis.org/A230000
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   not stated
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for the numerator polynomials:
 *   u(0) = 1
 *   u(1) = 1 + x
 *   u(n) = u(n-1) + x^(2n-1) * u(n-2)   for n ≥ 2
 *
 *  a(n) is the coefficient of x^k in u(row r), where r is the
 *  unique integer with triangular number T(r)=r(r+1)/2 ≤ n < T(r+1)+r+1.
 *  The sequence is indexed starting at n=0. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Determine the row r and position k within that row. */
    idx_t r = 0;
    idx_t acc = 0;            /* number of elements processed so far      */
    while (1) {
        idx_t tri = r * (r + 1) / 2;
        idx_t len = tri + 1;  /* elements in this row                     */
        if (acc + len > n) break;
        acc += len;
        ++r;
    }
    idx_t k = n - acc;        /* index within row r                        */

    /* Handle row 0 separately. */
    seq_t *prev2 = (seq_t *)malloc(sizeof(seq_t));
    if (!prev2) return 0;
    prev2[0] = (seq_t)1;
    idx_t len2 = 1;

    if (r == 0) {
        seq_t val = prev2[k];
        free(prev2);
        return val;
    }

    /* Row 1 */
    seq_t *prev1 = (seq_t *)malloc(2 * sizeof(seq_t));
    if (!prev1) { free(prev2); return 0; }
    prev1[0] = (seq_t)1;
    prev1[1] = (seq_t)1;
    idx_t len1 = 2;

    if (r == 1) {
        seq_t val = prev1[k];
        free(prev1);
        free(prev2);
        return val;
    }

    /* Build rows 2 … r using the recurrence */
    for (idx_t i = 2; i <= r; ++i) {
        idx_t tri = i * (i + 1) / 2;
        idx_t len = tri + 1;
        seq_t *cur = (seq_t *)calloc((size_t)len, sizeof(seq_t));
        if (!cur) {
            free(prev1);
            free(prev2);
            return 0;
        }

        /* copy u(i-1) */
        for (idx_t j = 0; j < len1; ++j)
            cur[j] = prev1[j];

        /* add shifted u(i-2) */
        idx_t shift = 2 * i - 1;
        for (idx_t j = 0; j < len2; ++j)
            cur[shift + j] += prev2[j];

        /* free previous rows and advance */
        free(prev2);
        prev2 = prev1;
        len2  = len1;

        prev1 = cur;
        len1  = len;
    }

    /* Retrieve the desired coefficient and clean up */
    seq_t result = prev1[k];
    free(prev1);
    free(prev2);
    return result;
}
