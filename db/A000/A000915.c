/*
 * A000915  Stirling numbers of first kind s(n+4, n).
 * https://oeis.org/A000915
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f. x*(24 + 58*x + 22*x^2 + x^3)/(1 - x)^9, i.e. a(n) =
 *            24*C(n+7,8) + 58*C(n+6,8) + 22*C(n+5,8) + C(n+4,8)
 * Range:     u8 n <= 1, u16 n <= 6, u32 n <= 31, u64 n <= 536, u128 n <= 1000,
 *            big any n
 * Helpers:   a000915_c8
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000915_c8.h"

seq_t a(idx_t n) {
    seq_t r = (seq_t)24 * a000915_c8(n + 7);
    r += (seq_t)58 * a000915_c8(n + 6);
    r += (seq_t)22 * a000915_c8(n + 5);
    r += a000915_c8(n + 4);
    return r;
}
