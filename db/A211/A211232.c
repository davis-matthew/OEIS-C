/*
 * A211232  Irregular triangle read by rows: T(n,k) is the k-th generalized
 *          Eulerian number of order n and degree 2, for n >= 1 (the rows
 *          start at k=1).
 * https://oeis.org/A211232
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, tabf
 * Formula:   not stated
 * Range:     i8 n <= 20, i16 n <= 54, i32 n <= 129, i64 n <= 305,
 *            i128 n <= 866, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Triangle read by rows (OEIS A211232).
 * T(1,1)=1, T(1,2)=2.
 * Recurrence for n>1:
 *   T(n,k) = k*T(n-1,k) - (n-k)*T(n-1,k-1) - (2*n-k)*T(n-1,k-2).
 * The sequence a(n) is the entries of T read row‑by‑row starting
 * with n=1.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)        /* 0 is outside the defined offset, return 0 */
        return (seq_t)0;

    /* Find the row r (n≥1) and column k such that the n‑th triangle
     * element lies in row r (1‑based).  The cumulative number of
     * elements up to row r is: 2 for r=1 and r²+1 for r≥2.  We
     * simply count rows until we reach or exceed n. */
    idx_t r = 1;                   /* current row          */
    idx_t cumulative = 2;          /* elements up to row 1 */
    while (n > cumulative) {
        ++r;
        cumulative += (r >= 2) ? (2 * r - 1) : 2;   /* add length of row r */
    }

    /* Length of the found row */
    idx_t row_len = (r == 1) ? 2 : 2 * r - 1;

    /* Column k (1‑based within the row) */
    idx_t k = n - (cumulative - row_len);   /* count elements before row r */

    /* Handle row 1 directly (no recurrence needed) */
    if (r == 1)
        return (k == 1) ? (seq_t)1 : (seq_t)2;

    /* For r≥2, build the triangle iteratively up to row r. */
    idx_t max_len = 2 * r - 1;                /* maximum row size */
    seq_t *prev = (seq_t *)calloc((size_t)(max_len + 1), sizeof(seq_t));
    seq_t *cur  = (seq_t *)calloc((size_t)(max_len + 1), sizeof(seq_t));
    if (!prev || !cur) {                     /* allocation failure */
        free(prev);
        free(cur);
        return (seq_t)0;
    }

    /* Initialize row 1 */
    prev[0] = 0;
    prev[1] = 1;
    prev[2] = 2;
    idx_t prev_len = 2;                       /* logical length of prev row */

    /* Construct rows 2 … r */
    for (idx_t m = 2; m <= r; ++m) {
        idx_t cur_len = 2 * m - 1;            /* length of current row */

        /* Zero out cur array to ensure unused entries are 0 */
        for (idx_t i = 0; i <= max_len; ++i)
            cur[i] = 0;

        cur[0] = 0;
        for (idx_t j = 1; j <= cur_len; ++j) {
            seq_t term1 = (seq_t)j * (j <= prev_len ? prev[j] : 0);
            seq_t term2 = ((seq_t)m - (seq_t)j) *
                         ((j - 1) <= prev_len ? prev[j - 1] : 0);
            seq_t term3 = 0;
            if (j >= 2) {
                seq_t coef = (seq_t)(2 * (seq_t)m - (seq_t)j);
                seq_t val  = (j - 2 <= prev_len ? prev[j - 2] : 0);
                term3 = coef * val;
            }
            cur[j] = term1 - term2 - term3;
        }

        /* Prepare for next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur = tmp;
        prev_len = cur_len;
    }

    seq_t result = prev[k];   /* prev now holds row r */

    free(prev);
    free(cur);
    return result;
}
