/*
 * A189480  [4rn]-4[rn], where r=sqrt(5) and [ ]=floor.
 * https://oeis.org/A189480
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(4*n*sqrt(5)) - 4*floor(n*sqrt(5))
 * Range:     u8 none, u16 n <= 14, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    /* n is 1-based; behavior for n < 1 is not required */
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;          /* n^2  (fits in seq_t for the n values tested) */
    seq_t f_n   = floor_sqrt((seq_t)5  * n2);   /* floor(n*sqrt(5)) */
    seq_t f_4n  = floor_sqrt((seq_t)80 * n2);   /* floor(4*n*sqrt(5)) */
    return f_4n - (seq_t)4 * f_n;
}
