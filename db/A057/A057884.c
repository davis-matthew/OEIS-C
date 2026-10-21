/*
 * A057884  A square array based on tetrahedral numbers (A000292) with each
 *          term being the sum of 2 consecutive terms in the previous row.
 * https://oeis.org/A057884
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A057884:
 * The array T(n,k) is defined by
 *   T(0,k) = T(4,k/2) if k is even, otherwise 0   (tetrahedral numbers)
 *   T(n,0) = 1   for n > 0
 *   T(n,k) = T(n-1,k-1) + T(n,k-1)   for k > 0
 *
 * a(n) is obtained by reading T(n,k) along anti‑diagonals:
 *   let d be the unique integer such that d*(d+1)/2 ≤ n < (d+1)*(d+2)/2
 *   let offset = n - d*(d+1)/2
 *   then a(n) = T(offset , d-offset)
 *
 * This function implements exactly that construction. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* find diagonal d such that d*(d+1)/2 ≤ n < (d+1)*(d+2)/2 */
    idx_t d = 0;
    while ((d + 1) * (d + 2) / 2 <= n)
        ++d;

    idx_t offset = n - d * (d + 1) / 2;
    idx_t i = offset;           /* row index */
    idx_t j = d - offset;       /* column index */

    /* maximum j needed for this n */
    size_t len = (size_t)j + 1;
    if (len == 0)
        len = 1;  /* avoid zero-size allocation */

    /* previous and current row buffers */
    seq_t *prev = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *curr = (seq_t *)calloc(len, sizeof(seq_t));

    /* initialise row 0: T(0,k) = T(4,k/2) if k even, else 0 */
    for (size_t k = 0; k < len; ++k) {
        if ((k & 1u) == 0) {          /* k even */
            idx_t half = k / 2;
            /* tetrahedral number C(half+3, 3) */
            seq_t val = ((seq_t)(half + 3)) *
                        ((seq_t)(half + 2)) *
                        ((seq_t)(half + 1)) / (seq_t)6;
            prev[k] = val;
        } else {
            prev[k] = (seq_t)0;
        }
    }

    /* compute rows 1 ... i */
    for (idx_t r = 1; r <= i; ++r) {
        curr[0] = (seq_t)1;           /* T(n,0) = 1 for n>0 */
        for (size_t k = 1; k < len; ++k) {
            curr[k] = prev[k - 1] + prev[k];
        }
        seq_t *tmp = prev;
        prev = curr;
        curr = tmp; /* swap buffers */
    }

    seq_t result = prev[j];

    free(prev);
    free(curr);

    return result;
}
