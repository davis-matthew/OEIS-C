/*
 * A154959  Triangle read by rows. Signed version of A008277.
 * https://oeis.org/A154959
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabl
 * Formula:   a(n) = (c==r ? +1 : -1) * S(r,c) where S(r,c) is the Stirling
 *            number of the second kind and (r,c) is the (row, column)
 *            position of the term in the triangular array read row‑wise
 * Range:     i8 n <= 23, i16 n <= 45, i32 n <= 45, i64 n <= 45, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;        /* offset is 1; out‑of‑range value */

    /* Find the row r such that triangular(r‑1) < n ≤ triangular(r) */
    idx_t r = 0;
    seq_t tri = 0;
    for (;;) {
        ++r;
        tri = (seq_t)r * (r + 1) / 2;
        if (tri >= n) break;
    }

    seq_t tri_prev = (seq_t)(r - 1) * r / 2;
    idx_t c = (idx_t)(n - tri_prev);     /* column within the row */

    /* Dynamic programming for Stirling numbers of the second kind.
       Only two rows are kept at a time. */
    seq_t *prev = (seq_t *)malloc((r + 2) * sizeof(seq_t));
    seq_t *cur  = (seq_t *)malloc((r + 2) * sizeof(seq_t));
    if (!prev || !cur) {
        free(prev); free(cur);
        return 0; /* out of memory */
    }

    /* Base case: row 1 */
    prev[1] = 1;
    if (r == 1) {
        seq_t result = (c == r) ? prev[1] : -prev[1];
        free(prev); free(cur);
        return result;
    }

    /* Compute rows 2 … r */
    for (idx_t i = 2; i <= r; ++i) {
        cur[1] = 1;               /* S(i,1) = 1 */
        for (idx_t k = 2; k < i; ++k)
            cur[k] = (seq_t)k * prev[k] + prev[k - 1]; /* recurrence */
        cur[i] = 1;             /* S(i,i) = 1 */

        /* swap the pointers for next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
    }

    seq_t value = prev[c];
    seq_t sign  = (c == r) ? (seq_t)1 : (seq_t)-1;
    seq_t result = value * sign;

    free(prev);
    free(cur);
    return result;
}
