/*
 * A026105  Triangle T read by rows: differences of Motzkin triangle
 *          (A026300).
 * https://oeis.org/A026105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is obtained by reading the triangle T(i,j) by rows.
 *            T(i,0)=1; T(1,1)=1; T(2,1)=T(2,2)=1; T(3,1)=2, T(3,2)=3,
 *            T(3,3)=2; For i>=3, j>=1: T(i,j) = T(i-1,j-2) + T(i-1,j-1) +
 *            T(i-1,j) (missing indices are 0). The sequence a(n) is T(r,k)
 *            where r is the row containing n in the flattened row‑by‑row...
 * Range:     u8 n <= 40, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find the row r such that cumulative number of terms up to row r
       exceeds n.  Cumulative terms up to row r are (r+1)(r+2)/2. */
    idx_t r = 0;
    idx_t cumulative = 1;          /* after row 0 */
    while (cumulative <= n) {
        r++;
        cumulative += r + 1;       /* add size of new row r */
    }

    /* If the first row only, return 1 immediately. */
    if (r == 0) {
        return (seq_t)1;
    }

    /* Determine index within row r. */
    idx_t prev_cumulative = cumulative - (r + 1);   /* terms up to row r-1 */
    idx_t k = n - prev_cumulative;                  /* 0‑based within row r */

    /* Build rows up to r, keeping only the previous row in memory. */
    seq_t *prev = (seq_t *)malloc(sizeof(seq_t) * 1);
    if (!prev) return 0;      /* allocation failed; silence compiler */

    prev[0] = (seq_t)1;       /* row 0 */

    for (idx_t i = 1; i <= r; ++i) {
        seq_t *curr = (seq_t *)malloc(sizeof(seq_t) * (i + 1));
        if (!curr) { free(prev); return 0; }

        curr[0] = (seq_t)1;   /* first element of every row is 1 */

        if (i == 1) {
            curr[1] = (seq_t)1;
        } else if (i == 2) {
            curr[1] = (seq_t)1;
            curr[2] = (seq_t)1;
        } else {
            /* General recurrence for i >= 3 */
            for (idx_t j = 1; j <= i; ++j) {
                seq_t a1 = (j >= 2 && j-2 < i)   ? prev[j-2] : (seq_t)0;
                seq_t a2 = (j >= 1 && j-1 < i)   ? prev[j-1] : (seq_t)0;
                seq_t a3 = (j < i)               ? prev[j]   : (seq_t)0;
                curr[j] = a1 + a2 + a3;
            }
        }

        free(prev);
        prev = curr;
    }

    /* prev now holds row r; fetch the k‑th element. */
    seq_t result = prev[k];
    free(prev);
    return result;
}
