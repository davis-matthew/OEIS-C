/*
 * A005830  a(n) = floor(phi*a(n-1)) + a(n-2) where phi is the golden ratio.
 * https://oeis.org/A005830
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(phi*a(n-1)) + a(n-2), a(0) = 1, a(1) = 3.
 *            floor(phi*x) is computed exactly with integers: in the big build
 *            as floor((x + isqrt(5*x^2))/2); in fixed-width builds via the
 *            Zeckendorf shift: if x = sum F(k_i), then floor(phi*x) = sum
 *            F(k_i+1) - [k_min even]
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 29, u64 n <= 59, u128 n <= 119,
 *            big any n
 * Helpers:   floor_phi_zeck, grow_zero, isqrt_from, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/grow_zero.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/floor_phi_zeck.h"
#include "../utils/isqrt_from.h"

static seq_t *va = 0, *vs = 0;
static idx_t vlen = 0, vcap = 0;

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 3;

    if (seq_is_fixed_width()) {
        seq_t p2 = 1, p1 = 3, c;
        idx_t i;
        for (i = 2; i <= n; i++) {
            c = floor_phi_zeck(p1) + p2;
            p2 = p1;
            p1 = c;
        }
        return p1;
    }

    if (vlen == 0) {
        vcap = 64;
        va = grow_zero(va, 0, vcap);
        vs = grow_zero(vs, 0, vcap);
        va[0] = 1; vs[0] = 2;   /* floor(sqrt(5)*1) */
        va[1] = 3; vs[1] = 6;   /* floor(sqrt(5)*3) */
        vlen = 2;
    }
    if (n >= vcap) {
        idx_t nc = vcap;
        while (nc <= n) nc *= 2;
        va = grow_zero(va, vcap, nc);
        vs = grow_zero(vs, vcap, nc);
        vcap = nc;
    }
    while (vlen <= n) {
        idx_t i = vlen;
        seq_t x = va[i - 1];
        seq_t v = (x + vs[i - 1]) / 2 + va[i - 2];
        seq_t N = v * v * 5;
        seq_t g = (vs[i - 1] + 1) * v / x + 1;
        va[i] = v;
        vs[i] = isqrt_from(N, g);
        vlen++;
    }
    return va[n];
}
