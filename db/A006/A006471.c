/*
 * A006471  Number of tree-rooted planar maps with 4 faces and n vertices and
 *          no isthmuses.
 * https://oeis.org/A006471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5*C(n+8,9) + 34*C(n+7,9) + 35*C(n+6,9) + 4*C(n+5,9), from
 *            G.f.: x*(4*x^3 + 35*x^2 + 34*x + 5) / (1-x)^10
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 27, u64 n <= 350, u128 n <= 1000,
 *            big any n
 * Helpers:   a006471_c9, a006471_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a006471_c9.h"
#include "../utils/a006471_gcd.h"

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;
    seq_t r = 0;
    r += a006471_c9(m + 8) * 5;
    r += a006471_c9(m + 7) * 34;
    r += a006471_c9(m + 6) * 35;
    r += a006471_c9(m + 5) * 4;
    return r;
}
