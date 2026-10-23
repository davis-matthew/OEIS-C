/*
 * A131507  2n+1 appears n+1 times.
 * https://oeis.org/A131507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2*floor(sqrt(2n+1)+1/2) - 1
 * Range:     u8 n <= 31, u16 n <= 8000, u32 n <= 8000, u64 n <= 8000,
 *            u128 n <= 8000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* y = 2*n + 1 */
    seq_t y = ((seq_t)n << 1) + (seq_t)1;

    /* Compute floor sqrt of y */
    seq_t s   = floor_sqrt(y);

    /* Check whether we should round up */
    seq_t s2  = s * s;
    seq_t m   = s;
    if (y >= s2 + s + (seq_t)1)
        m = s + (seq_t)1;

    /* a(n) = 2*m – 1 */
    return m * (seq_t)2 - (seq_t)1;
}
