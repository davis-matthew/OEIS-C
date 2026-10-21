/*
 * A051598  Rows of triangle formed using Pascal's rule except begin n-th row
 *          with 2*n and end it with 2*n+1.
 * https://oeis.org/A051598
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) is the n‑th element of the triangle obtained by applying
 *            Pascal’s rule to each row but with the first element of row r
 *            equal to 2*r and the last element equal to 2*r+1 (row 0 is the
 *            special single element 1). The sequence is defined for n>=0 with
 *            a(0)=1
 * Range:     u8 n <= 39, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Number of columns needed for n up to 10000:
   r satisfies (r+1)(r+2)/2 > n, so r <= 141.  We use a small
   statically sized buffer. */
#define MAX_R 200

seq_t a(idx_t n)
{
    /* Special first term */
    if (n == 0)
        return (seq_t)1;

    /* Find the row r that contains index n.
       S_r = (r+1)(r+2)/2 is the number of terms up to and including row r.
       We want the smallest r with S_r > n. */
    idx_t r = 0;
    while (((r + 1) * (r + 2) / 2) <= n)
        ++r;

    /* Index of the term within row r.  S_{r-1} = r(r+1)/2. */
    idx_t idx_in_row = n - (r * (r + 1) / 2);

    /* Work arrays for two consecutive rows. */
    seq_t prev[MAX_R];
    seq_t curr[MAX_R];

    /* Base row 0 */
    prev[0] = (seq_t)1;

    /* Build rows from 1 up to r.  For each row we only need the
       previous one, so O(r) memory and time. */
    for (idx_t i = 1; i <= r; ++i) {
        curr[0] = (seq_t)(2 * i);           /* first element of row i */
        curr[i] = (seq_t)(2 * i + 1);       /* last element of row i */

        /* Interior elements via Pascal's rule.  Use 128‑bit
           intermediate to avoid overflow on 64‑bit builds. */
        for (idx_t j = 1; j < i; ++j) {
            curr[j] = (seq_t)(((__uint128_t)prev[j - 1] + prev[j]));
        }

        /* Prepare for next iteration. */
        for (idx_t j = 0; j <= i; ++j)
            prev[j] = curr[j];
    }

    /* Return the required element from the final row. */
    return curr[idx_in_row];
}
