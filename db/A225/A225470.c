/*
 * A225470  Triangle read by rows, s_3(n, k) where s_m(n, k) are the
 *          Stirling-Frobenius cycle numbers of order m; n >= 0, k >= 0.
 * https://oeis.org/A225470
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A225470 – Stirling–Frobenius cycle numbers of order 3
 *
 * a(n) is obtained by reading the lower‑triangular array T(n,k) = s_3(n,k)
 * row‑by‑row (rows indexed from 0).  The recurrence for the triangle is
 *    T(0,0) = 1
 *    T(i,j) = (2 + 3*(i-1)) * T(i-1,j) + T(i-1,j-1)    for 1 ≤ j ≤ i-1
 *    T(i,0) = (2 + 3*(i-1)) * T(i-1,0)
 *    T(i,i) = 1
 *
 * For a given n the indices (row r, column k) satisfy
 *    r*(r+1)/2 ≤ n < (r+1)*(r+2)/2,  k = n - r*(r+1)/2.
 *
 * The implementation computes the required row by dynamic programming.
 * It works for any seq_t that supports the integer operations listed
 * in the problem statement.
 */

#include <stdlib.h>

/* compute a(n) */
seq_t a(idx_t n)
{
    /* find the row r that contains the n-th term */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* column within the row */
    idx_t k = n - r * (r + 1) / 2;

    if (r == 0)                /* row 0 contains only the value 1 */
        return (seq_t)1;

    /* allocate two temporary rows of size r+1 */
    seq_t *prev = (seq_t *)calloc(r + 1, sizeof(seq_t));
    seq_t *cur  = (seq_t *)calloc(r + 1, sizeof(seq_t));
    if (!prev || !cur) {       /* allocation failed – abort */
        free(prev);
        free(cur);
        return (seq_t)0;
    }

    /* initialise row 0 */
    prev[0] = (seq_t)1;

    /* build successive rows up to row r */
    for (idx_t i = 1; i <= r; ++i) {
        seq_t a_val = (seq_t)(2 + 3 * (i - 1));   /* coefficient a_i */

        /* j = 0 case */
        cur[0] = prev[0] * a_val;

        /* j from 1 to i */
        for (idx_t j = 1; j <= i; ++j) {
            if (j == i)
                cur[j] = (seq_t)1;                /* diagonal element */
            else
                cur[j] = prev[j] * a_val + prev[j - 1];
        }

        /* swap prev and cur for next iteration */
        seq_t *tmp = prev;
        prev = cur;
        cur  = tmp;
    }

    seq_t result = prev[k];        /* a(n) */
    free(prev);
    free(cur);
    return result;
}
