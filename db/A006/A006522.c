/*
 * A006522  4-dimensional analog of centered polygonal numbers. Also number of
 *          regions created by sides and diagonals of a convex n-gon in
 *          general position.
 * https://oeis.org/A006522
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = binomial(n,4) + binomial(n-1,2), with a(0) = 1
 * Range:     u8 n <= 10, u16 n <= 36, u32 n <= 568, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   small_choose
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/small_choose.h"

seq_t a(idx_t n) {
    if (n == 0) return 1;
    return small_choose(n, 4) + small_choose(n - 1, 2);
}
