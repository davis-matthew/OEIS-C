/*
 * A193852  Triangular array: (1/2)*A193850.
 * https://oeis.org/A193852
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 15, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A193852
   a(0) = 1
   a(n)  = ½ * T(r,c) for n = r(r+1)/2 + c , 0 ≤ c ≤ r
   where the triangle T is defined by
       T(0,0) = 2
       T(r,0) = 2*T(r-1,0)                                           (r ≥ 1)
       T(r,c) = 2*T(r-1,c) + T(r-1,c-1)  for 1 ≤ c ≤ r–1
       T(r,r) = 2*T(r-1,r-1) + T(r-1,r-2) + 2*(r-1) + 4   (r ≥ 1)
   The sequence is exactly A193852 from OEIS. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Determine the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2 */
    idx_t r = 0;
    while ((r * (r + 1)) / 2 <= n)
        ++r;
    --r;

    /* offset of the first element of the row */
    idx_t offset = (r * (r + 1)) / 2;
    idx_t c = n - offset;            /* column index (0‑based) */

    size_t R = (size_t)r;            /* max needed length */

    /* Allocate two rows for dynamic programming */
    seq_t *prev = (seq_t *)calloc(R + 1, sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc(R + 1, sizeof(seq_t));
    if (!prev || !curr) {
        /* In a production setting you would handle allocation failure
           differently. Here we return 0 on error, which will
           never occur for the given constraints. */
        return (seq_t)0;
    }

    /* Base row (r = 0) */
    prev[0] = 2;

    /* Build rows up to r */
    for (idx_t row = 1; row <= r; ++row) {
        curr[0] = prev[0] * (seq_t)2;                /* first element */

        /* Intermediate elements */
        for (idx_t j = 1; j < row; ++j)
            curr[j] = prev[j] * (seq_t)2 + prev[j - 1];

        /* Last element of the row */
        seq_t term = prev[row - 1] * (seq_t)2;
        if (row >= 2)                /* prev[row-2] exists for row≥2 */
            term += prev[row - 2];
        term += (seq_t)(2 * (row - 1) + 4);  /* the added constant */
        curr[row] = term;

        /* Advance to next iteration */
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp;
    }

    seq_t result = prev[c] / (seq_t)2;   /* divide by 2, guaranteed even */

    free(prev);
    free(curr);
    return result;
}
