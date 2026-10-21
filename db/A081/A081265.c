/*
 * A081265  Triangle of coefficients of the polynomials a(n, x) = 2*a(n-1, x)+
 *          x^2*a(n-2,x), n >= 1, a(0, x) = 1, a(1, x) = 1.
 * https://oeis.org/A081265
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = coefficient of the polynomial a(row,x) in the triangle
 *            defined by a(0,x) = 1, a(1,x) = 1, a(n,x) = 2*a(n-1,x) + x^2 *
 *            a(n-2,x) (n ≥ 2), enumerated in row‑major order (flat list of
 *            all coefficients). The function returns the nth element of this
 *            sequence
 * Range:     u8 n <= 37, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row that contains index n.  The number of
       elements before row r is r*(r+1)/2.  Solve r(r+1)/2 ≤ n. */
    idx_t low = 0, high = n;
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        idx_t cum = mid * (mid + 1) / 2;
        if (cum <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r   = low;                         /* row number   */
    idx_t start = r * (r + 1) / 2;           /* index of first element of row r */
    idx_t k   = n - start;                   /* column inside the row (0‑based) */

    /* Special cases: row 0 and row 1 */
    if (r == 0)
        return (seq_t)1;                     /* only one element, always 1 */

    /* Allocate arrays that will hold rows up to length r+1. */
    const size_t max_len = (size_t)r + 1;
    seq_t *prev2 = (seq_t *)calloc(max_len, sizeof(seq_t)); /* row i-2 */
    seq_t *prev1 = (seq_t *)calloc(max_len, sizeof(seq_t)); /* row i-1 */
    seq_t *cur   = (seq_t *)calloc(max_len, sizeof(seq_t)); /* row i   */

    /* Initialize the first two rows.  The calloc gives zeros,
       so we only set the non‑zero entries. */
    prev2[0] = (seq_t)1;          /* a(0,x) = 1 */
    prev1[0] = (seq_t)1;          /* a(1,x) = 1 */
    /* prev1[1] stays 0 */

    if (r == 1) {
        seq_t result = prev1[k];
        free(prev2); free(prev1); free(cur);
        return result;
    }

    size_t len2 = 1;   /* length of prev2 (row 0)   */
    size_t len1 = 2;   /* length of prev1 (row 1)   */

    /* Build rows 2 .. r using the recurrence      */
    for (idx_t i = 2; i <= r; ++i) {
        size_t cur_len = (size_t)i + 1;

        /* Compute the current row coefficients.        */
        for (size_t j = 0; j < cur_len; ++j) {
            seq_t val = (seq_t)0;
            if (j < len1)                 /* from 2*a_{i-1,x}   */
                val += (seq_t)2 * prev1[j];
            if (j >= 2 && (j - 2) < len2) /* from a_{i-2,x} shifted by 2 */
                val += prev2[j - 2];
            cur[j] = val;
        }

        /* Rotate the buffers:  prev2 <- prev1, prev1 <- cur, cur <- old prev2 */
        seq_t *tmp  = prev2;
        prev2 = prev1;
        prev1 = cur;
        cur   = tmp;
        len2 = len1;
        len1 = cur_len;
    }

    /* After the loop, prev1 holds row r. Extract the desired coefficient. */
    seq_t result = prev1[k];

    /* Clean up allocated memory. */
    free(prev2); free(prev1); free(cur);
    return result;
}
