/*
 * A055086  n appears 1+[n/2] times.
 * https://oeis.org/A055086
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(sqrt(4*n + 1)) - 1
 * Range:     u8 n <= 15, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements OEIS A055086. */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Special case n == 0 is handled correctly by the formula below. */
    seq_t v = (seq_t)n * 4 + 1;  /* v = 4*n + 1   (all in seq_t) */
    seq_t s = floor_sqrt(v);    /* s = ⌊√v⌋      */
    return s - 1;               /* a(n) = s - 1  */
}
