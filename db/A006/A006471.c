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
 * Validated: standard, pass, 46 terms through n = 948 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_u64.h"

/* C(m,9) exactly: cancel 9! against the factors first, then only multiply */
static seq_t a006471_c9(uint64_t m) {
    uint64_t nums[9];
    int i, j;
    seq_t r = 1;
    if (m < 9) return 0;
    for (i = 0; i < 9; i++) nums[i] = m - (uint64_t)i;
    for (j = 2; j <= 9; j++) {
        uint64_t g = (uint64_t)j;
        for (i = 0; i < 9 && g > 1; i++) {
            uint64_t d = gcd_u64(nums[i], g);
            nums[i] /= d;
            g /= d;
        }
    }
    for (i = 0; i < 9; i++) r *= (seq_t)nums[i];
    return r;
}

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n;
    seq_t r = 0;
    r += a006471_c9(m + 8) * 5;
    r += a006471_c9(m + 7) * 34;
    r += a006471_c9(m + 6) * 35;
    r += a006471_c9(m + 5) * 4;
    return r;
}
