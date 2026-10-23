/*
 * A184012  n + floor(sqrt(5n-4)); complement of A184013.
 * https://oeis.org/A184012
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt(5n-4))
 * Range:     u8 n <= 13, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* 5*n may overflow 64-bit, but seq_t is at least 64 bits, so cast to seq_t first. */
    seq_t val = (seq_t)5 * (seq_t)n - (seq_t)4;
    seq_t root = floor_sqrt(val);
    return (seq_t)n + root;
}
