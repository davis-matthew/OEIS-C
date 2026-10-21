/*
 * A209134  Triangle of coefficients of polynomials v(n,x) jointly generated
 *          with A209133; see the Formula section.
 * https://oeis.org/A209134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) where (r,k) is the unique pair such that
 *            sum_{i=1}^{r-1} i < n ≤ sum_{i=1}^{r} i, and T is defined by
 *            T(1,0)=1, T(2,0)=0, T(2,1)=4, and for r≥3
 *            T(r,k)=T(r-1,k)+2*T(r-1,k-1)+2*T(r-2,k-2) (with T(r,k)=0 when
 *            k<0 or k≥r). The function returns the n‑th element of the
 *            flattened triangle...
 * Range:     u8 n <= 19, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* guard for non‑positive indices – the offset is 1 */
    if (n < 1) return 0;

    /* Find the row r and column k corresponding to the index n. */
    idx_t r = 0, sum = 0;
    while (sum < n) {
        ++r;
        sum += r;                 /* sum of sizes of rows 1..r */
    }
    idx_t sum_prev = sum - r;      /* sum of sizes of rows 1..r-1 */
    idx_t k = n - sum_prev - 1;    /* 0‑based column index in row r */

    /* Handle the first two rows directly. */
    if (r == 1) return 1;          /* T(1,0)=1 */
    if (r == 2) return (k == 0) ? 0 : 4;  /* T(2,0)=0, T(2,1)=4 */

    /* Prepare rows 1 and 2 for the recurrence. */
    seq_t *prev2 = (seq_t *)malloc(sizeof(seq_t) * 1);   /* row 1 */
    prev2[0] = 1;

    seq_t *prev1 = (seq_t *)malloc(sizeof(seq_t) * 2);   /* row 2 */
    prev1[0] = 0;   /* T(2,0) */
    prev1[1] = 4;   /* T(2,1) */

    /* Build rows 3..r using the recurrence. */
    for (idx_t row = 3; row <= r; ++row) {
        seq_t *curr = (seq_t *)malloc(sizeof(seq_t) * row);
        for (idx_t kk = 0; kk < row; ++kk) {
            seq_t a_val = (kk < row - 1) ? prev1[kk] : 0;
            seq_t b_val = (kk > 0)     ? prev1[kk - 1] : 0;
            seq_t c_val = (kk >= 2)    ? prev2[kk - 2] : 0;
            curr[kk] = a_val + 2 * b_val + 2 * c_val;
        }
        /* Advance the row buffers: prev2 <- prev1, prev1 <- curr. */
        free(prev2);
        prev2 = prev1;
        prev1 = curr;
    }

    /* Extract the desired entry from the last computed row. */
    seq_t result = prev1[(size_t)k];
    free(prev2);
    free(prev1);
    return result;
}
