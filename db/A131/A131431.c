/*
 * A131431  3n + 1 preceded by n zeros.
 * https://oeis.org/A131431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 3r+1 if n = r(r+3)/2 for some r >= 0; otherwise 0
 * Range:     u8 n <= 6, u16 n <= 788, u32 n <= 9808, u64 n <= 9808,
 *            u128 n <= 9808, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* Compute discriminant D = 9 + 8n */
    seq_t D = (seq_t)8 * n + 9;
    /* Compute integer sqrt of D */
    seq_t s = floor_sqrt(D);
    /* If D is not a perfect square, a(n) is 0 */
    if (s * s != D)
        return 0;

    /* Now check if s-3 is an even non‑negative number */
    if (s < 3)
        return 0;
    seq_t diff = s - 3;
    if (diff & 1)
        return 0;  /* not divisible by 2 */

    seq_t r = diff >> 1;   /* r = (s-3)/2 */
    return 3 * r + 1;
}
