/*
 * A125235  Triangle with the partial column sums of the octagonal numbers.
 * https://oeis.org/A125235
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   not stated
 * Range:     u8 n <= 29, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A125235: Triangle with the partial column sums of the octagonal numbers.
   Implementation follows the triangle recurrence:
     T(n,1) = n * (3*n - 2)          // octagonal numbers
     T(n,k) = T(n-1,k-1) + T(n-1,k),   k > 1
   The sequence a(n) is the flattened row‑major enumeration of the triangle,
   with a(1) = T(1,1). This routine computes a(n) for any n ≥ 1.
   Complexity: O(r²) where r ≈ √(2n); r ≤ 141 for n ≤ 10 000.
   No memoisation is needed; each call rebuilds only up to the required row. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Find row r such that sum_{i=1}^{r-1} i < n ≤ sum_{i=1}^{r} i. */
    size_t r = 1;
    size_t cum = 0;            /* number of elements in rows before r */
    while (cum + r < n) {
        cum += r;
        r++;
    }

    /* Offset within row r (0‑based). */
    size_t offset = n - cum - 1;

    seq_t *prev = NULL;        /* previous row */
    size_t prev_len = 0;

    for (size_t i = 1; i <= r; ++i) {
        seq_t *cur = (seq_t *)malloc(i * sizeof(seq_t));
        if (!cur) return (seq_t)0;  /* allocation failure (unlikely) */

        for (size_t k = 1; k <= i; ++k) {
            if (k == 1) {
                /* Octagonal number formula */
                cur[k - 1] = (seq_t)i * (((seq_t)(3 * i)) - 2);
            } else {
                seq_t v = prev[k - 2];
                if (k - 1 < prev_len) v += prev[k - 1];
                cur[k - 1] = v;
            }
        }

        if (i == r) {
            seq_t result = cur[offset];
            free(cur);
            if (prev) free(prev);
            return result;
        }

        /* Prepare for next iteration: cur becomes the new prev. */
        if (prev) free(prev);
        prev = cur;
        prev_len = i;
    }

    /* Should never reach here. */
    return (seq_t)0;
}
