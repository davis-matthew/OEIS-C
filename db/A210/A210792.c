/*
 * A210792  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A210791; see the Formula section.
 * https://oeis.org/A210792
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Triangle of coefficients of the polynomials v(n,x) generated
 *            with u(n,x) and v(n,x) (see the OEIS entry). The sequence a(n)
 *            is obtained by reading the triangle row‑by‑row (row 1 has 1
 *            entry, row 2 has 2, etc.). The recurrence for the triangle
 *            T(n,k) is: T(n,k) = 3*T(n-1,k) + T(n-1,k-1) - 2*T(n-2,...
 * Range:     u8 n <= 30, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;      /* below offset, not used by tests */

    /* find the row r (1‑based) and the column k (0‑based) for the
       given flattened index n */
    idx_t r = 0;
    idx_t cum = 0;
    while (cum + (r + 1) < n) {
        r++;
        cum += r;
    }
    r++;                    /* now r satisfies cum + r >= n   */
    idx_t k = n - cum - 1;   /* column index in the row r      */

    if (r == 1) return (seq_t)1;
    if (r == 2) return (k == 0) ? (seq_t)1 : (seq_t)2;

    /* allocate arrays of length r+1 for rows up to r */
    size_t len = (size_t)(r + 1);
    seq_t *prev2 = (seq_t*)calloc(len, sizeof(seq_t));   /* row n-2  */
    seq_t *prev1 = (seq_t*)calloc(len, sizeof(seq_t));   /* row n-1  */
    seq_t *curr  = (seq_t*)calloc(len, sizeof(seq_t));    /* current row */

    /* initialise the first two rows */
    prev2[0] = (seq_t)1;          /* row 1: 1 */
    prev1[0] = (seq_t)1;          /* row 2: 1, 2 */
    prev1[1] = (seq_t)2;

    /* compute rows 3 … r using the recurrence */
    for (idx_t row = 3; row <= r; ++row) {
        size_t row_len = (size_t)row;
        for (size_t j = 0; j < row_len; ++j) {
            seq_t t = 3 * prev1[j];
            if (j > 0)          t += prev1[j - 1];
            t -= 2 * prev2[j];
            if (j > 0)          t -= 2 * prev2[j - 1];
            if (j > 1)          t += prev2[j - 2];
            curr[j] = t;
        }
        /* rotate pointers for next iteration */
        seq_t *tmp = prev2;
        prev2 = prev1;
        prev1 = curr;
        curr  = tmp;
    }

    seq_t result = prev1[(size_t)k];

    /* free allocated memory */
    free(prev2);
    free(prev1);
    free(curr);

    return result;
}
