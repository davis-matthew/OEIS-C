/*
 * A006411  Number of nonseparable tree-rooted planar maps with n + 2 edges
 *          and 3 vertices.
 * https://oeis.org/A006411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n*(n+1)*(n+2)^2*(n+3)/24 = binom(n+3,4)*(n+2)
 * Range:     u8 n <= 4, u16 n <= 15, u32 n <= 157, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n) {
    return binom(n + 3, 4) * ((seq_t)n + 2);
}
