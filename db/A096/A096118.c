/*
 * A096118  Duplicate of A050029.
 * https://oeis.org/A096118
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 18, i16 n <= 24, i32 n <= 24, i64 n <= 24, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A096118 (duplicate of A050029)
 *
 * a(1) = a(2) = 1
 * a(3) = 2,  a(4) = 3,  a(5) = 4
 * For n ≥ 6
 *   let δ cycle through the list {2,4,6,8,10,12,14,16}
 *   if n-δ ≤ 0 the cycle is reset to δ = 2
 *   a(n) = a(n‑1) + a(n‑δ)
 *
 * The cycle proceeds in order; after using δ=16 it restarts at δ=2.
 *
 * This recurrence reproduces the first 24 terms
 *  1,1,2,3,4,7,9,10,11,21,30,37,41,44,46,47,48,95,141,185,226,263,293,314
 *
 * It runs in O(n) time and O(n) space for a single evaluation.
 */

#include <stdlib.h>   /* malloc, free */

seq_t a(idx_t n)
{
    if (n == 0u)  /* sequence is defined for n ≥ 1 */
        return 0;

    /* Allocate the array 1..n (index 0 unused). */
    seq_t *seq = (seq_t *)malloc((n + 1u) * sizeof(seq_t));
    if (!seq)
        return 0;   /* allocation failed */

    /* Base values. */
    seq[1] = 1;      /* a(1) */
    if (n >= 2) seq[2] = 1;      /* a(2) */
    if (n >= 3) seq[3] = 2;      /* a(3) */
    if (n >= 4) seq[4] = 3;      /* a(4) */
    if (n >= 5) seq[5] = 4;      /* a(5) */

    /* Offset cycling: 2,4,6,8,10,12,14,16 */
    static const idx_t offsets[8] = {2, 4, 6, 8, 10, 12, 14, 16};
    idx_t cycle = 0;            /* index into offsets */

    for (idx_t i = 6u; i <= n; ++i)
    {
        idx_t off = offsets[cycle];

        /* If the offset would point before index 1,
           restart the cycle at 2. */
        if (i <= off)
        {
            cycle = 0;
            off   = offsets[cycle];
        }

        seq[i] = seq[i - 1] + seq[i - off];

        /* Advance to the next offset in the cycle. */
        cycle = (cycle + 1u) % 8u;
    }

    seq_t result = seq[n];
    free(seq);
    return result;
}
