/*
 * A083854  Numbers that are squares, twice squares, three times squares, or
 *          six times squares, i.e., numbers whose squarefree part divides 6.
 * https://oeis.org/A083854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 686, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A083854
   a(n) is the nth non‑negative integer whose squarefree part divides 6,
   i.e. the sorted list of numbers of the form k*m² with k∈{1,2,3,6}
   (plus a(0)=0).  This implementation merges the four arithmetic
   progressions and runs in O(n) time. */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* a(0) = 0 */
    if (n == 0)
        return 0;

    /* number of terms to produce (including a(0)) */
    idx_t target = n;

    /* Allocate storage for all terms up to n */
    seq_t *vals = (seq_t *)malloc((size_t)(target + 1) * sizeof(seq_t));
    if (!vals)
        return 0;               /* out of memory sentinel */

    /* initial list: a(0) = 0 */
    vals[0] = 0;

    /* indices for the four sequences k = {1,2,3,6} */
    idx_t m[4] = {1, 1, 1, 1};
    const seq_t k[4] = {1, 2, 3, 6};
    seq_t next[4] = {1, 2, 3, 6};

    /* current count of stored terms (including the initial 0) */
    idx_t count = 1;

    while (count <= target) {
        /* find the smallest current candidate */
        size_t best = 0;
        seq_t best_val = next[0];
        for (size_t i = 1; i < 4; ++i) {
            if (next[i] < best_val) {
                best_val = next[i];
                best = i;
            }
        }

        /* store the smallest value */
        vals[count++] = best_val;

        /* advance the chosen sequence */
        m[best] += 1;
        seq_t mval = (seq_t)m[best];
        next[best] = k[best] * mval * mval;
    }

    seq_t result = vals[target];
    free(vals);
    return result;
}
