/*
 * A131333  A131332 * A000012.
 * https://oeis.org/A131333
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n-th element of the triangle A131332, reconstructed
 *            * by the recurrence: * * let prev[0..l-1] be row r-1 (with l =
 *            r-1). * form new row curr[0..l] (size l+1) as: * curr[0] =
 *            prev[0] + (prev[1] if l>=2 else 0) * for i = 1 .. l: * if (i &
 *            1) // odd i (2nd,4th,… element) * curr[i] = prev...
 * Range:     u8 n <= 55, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;                /* undefined indices return 0 */

    /* Find the row r that contains the n-th element */
    idx_t r = 1;
    while ((seq_t)r * (seq_t)(r + 1) / 2 < n) r++;

    /* Start with the first row: [1] */
    idx_t  prev_len = 1;
    seq_t *prev = (seq_t *)malloc(prev_len * sizeof(seq_t));
    prev[0] = (seq_t)1;

    /* Build rows up to row r */
    for (idx_t cur_row = 2; cur_row <= r; ++cur_row) {
        idx_t  curr_len = prev_len + 1;
        seq_t *curr = (seq_t *)malloc(curr_len * sizeof(seq_t));

        for (idx_t i = 0; i < curr_len; ++i) {
            if (i == 0) {
                curr[0] = prev[0] + (prev_len >= 2 ? prev[1] : (seq_t)0);
            } else if (i & 1) {          /* odd index */
                curr[i] = prev[i - 1];
            } else {                    /* even index */
                curr[i] = prev[i - 1] + (i < prev_len ? prev[i] : (seq_t)0);
            }
        }

        free(prev);
        prev = curr;
        prev_len = curr_len;
    }

    /* Compute offset within the row */
    seq_t tri_before = (seq_t)(r - 1) * (seq_t)r / 2;        /* T(r-1) */
    idx_t col_index = (idx_t)(n - tri_before - 1);           /* 0‑based */

    seq_t result = prev[col_index];
    free(prev);
    return result;
}
