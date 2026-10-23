/*
 * A135676  a(n) = floor(n^(4/3) - n).
 * https://oeis.org/A135676
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^(4/3) - n)
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 139, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: a(n) = floor_cube_root(n^4) - n. */
#include "../utils/floor_cube_root.h"

seq_t a(idx_t n) {
    /* For n >= 1 the result is guaranteed to be non‑negative and fit in seq_t. */
    seq_t nseq = (seq_t)n;            /* convert index to seq_t */
    seq_t n2   = nseq * nseq;         /* n^2 */
    seq_t n4   = n2 * n2;             /* n^4 */
    seq_t root = floor_cube_root(n4); /* floor(n^(4/3)) */
    return root - nseq;               /* floor(n^(4/3)) - n == floor(n^(4/3) - n) */
}
