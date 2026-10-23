/*
 * A183573  a(n) = n + floor(sqrt(2n+1)).
 * https://oeis.org/A183573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt(2*n + 1))
 * Range:     u8 n <= 31, u16 n <= 8085, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    seq_t nn = (seq_t)n;
    seq_t t = (nn << 1) + 1;   /* 2*n + 1, using shift for multiplication by 2 */
    seq_t s = floor_sqrt(t);   /* floor(sqrt(2*n + 1)) */
    return nn + s;             /* a(n) = n + floor(sqrt(2*n + 1)) */
}
