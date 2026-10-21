/*
 * A098003  Start with positive integers. On the m-th step, shift terms a(m+1)
 *          to a(2m-1) one position to the left, then move a(m) to position
 *          (2m-1). Sequence is limit of reordering.
 * https://oeis.org/A098003
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the limit of the permutation formed by iteratively
 *            moving the element currently at position m to position 2m‑1
 *            (shifting the segment m+1 … 2m‑1 left by one place). This
 *            routine simulates the process up to the m‑th step and returns
 *            a(n)
 * Range:     u8 n <= 191, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The sequence is fixed after the n‑th step, so we only need
       indices up to 2n‑1.  Use 1‑based indexing for clarity. */
    idx_t limit = 2 * n - 1;
    seq_t *seq = (seq_t *)malloc((size_t)(limit + 1) * sizeof(seq_t));
    if (!seq) return 0;                     /* allocation failure */

    for (idx_t i = 1; i <= limit; ++i)
        seq[i] = (seq_t)i;                  /* start with 1,2,3,… */

    for (idx_t m = 1; m <= n; ++m) {
        seq_t x   = seq[m];                  /* the element to move */
        idx_t end = 2 * m - 1;               /* target position */

        /* shift elements left from m+1 to end */
        for (idx_t i = m + 1; i <= end; ++i)
            seq[i - 1] = seq[i];

        seq[end] = x;                       /* insert at 2m‑1 */
    }

    seq_t result = seq[n];
    free(seq);
    return result;
}
