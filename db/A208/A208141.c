/*
 * A208141  Number of n X 7 0..1 arrays avoiding 0 0 1 and 0 1 1 horizontally
 *          and 0 0 1 and 1 0 1 vertically.
 * https://oeis.org/A208141
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 30, u64 n <= 210, u128 n <= 210,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence a(n) from OEIS A208141.
 *
 * Definition (conjectured by Colin Barker):
 *   a(1) = 20
 *   a(2) = 400
 *   a(3) = 3000
 *   a(4) = 14000
 *   a(5) = 49000
 *   a(6) = 141120
 *   a(7) = 352800
 *   a(8) = 792000
 *   for n > 8:
 *     a(n) = 8*a(n-1) - 28*a(n-2) + 56*a(n-3) - 70*a(n-4)
 *            + 56*a(n-5) - 28*a(n-6) + 8*a(n-7) - a(n-8)
 *
 * This implementation computes the value iteratively using only the
 * eight most recent terms, avoiding any dynamic allocation or extra
 * memory and working correctly for any seq_t type (including arbitrary
 * precision) and any idx_t type.
 */

#include <stddef.h>   /* for size_t (needed for array indexing) */

seq_t a(idx_t n)
{
    /* Initial values for n = 1 … 8.  Indices in the array are zero‑based. */
    static const seq_t init[8] = {
        20,        /* a(1) */
        400,       /* a(2) */
        3000,      /* a(3) */
        14000,     /* a(4) */
        49000,     /* a(5) */
        141120,    /* a(6) */
        352800,    /* a(7) */
        792000     /* a(8) */
    };

    /* Handle small n directly. */
    if (n >= 1 && n <= 8) {
        return init[n - 1];
    }

    /* Working array holding the last eight values:
     * prev[0] = a(k-7), prev[1] = a(k-6), …, prev[7] = a(k-1)
     */
    seq_t prev[8];
    for (idx_t i = 0; i < 8; ++i) {
        prev[i] = init[i];
    }

    /* Compute progressively up to the requested n. */
    for (idx_t k = 9; k <= n; ++k) {
        seq_t next =
            ((seq_t)8)  * prev[7]  -
            ((seq_t)28) * prev[6]  +
            ((seq_t)56) * prev[5]  -
            ((seq_t)70) * prev[4]  +
            ((seq_t)56) * prev[3]  -
            ((seq_t)28) * prev[2]  +
            ((seq_t)8)  * prev[1]  -
            ((seq_t)1)  * prev[0];

        /* Shift previous values left by one position. */
        for (idx_t j = 0; j < 7; ++j) {
            prev[j] = prev[j + 1];
        }
        prev[7] = next;   /* store a(k) */
    }

    /* After the loop, prev[7] holds a(n). */
    return prev[7];
}
