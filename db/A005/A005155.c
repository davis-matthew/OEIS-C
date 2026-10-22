/*
 * A005155  Number of degree sequences of n-node graphs.
 * https://oeis.org/A005155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (B_n(n) + C(n))/2, from the e.g.f. by Lagrange inversion,
 *            where C(n) = He_n(n+1) - n*He_{n-1}(n+1), He_0(y) = 1, He_1(y) =
 *            y, He_{m+1}(y) = y*He_m(y) - m*He_{m-1}(y); B_m(y) = m! [t^m]
 *            sqrt(1-t^2)*exp(y*t - t^2/2): B_0 = 1, B_{<0} = 0, B_{m+1} =
 *            y*B_m + m(m-3)*B_{m-1} - y*m(m-1)*B_{m-2} +...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 27,
 *            big any n
 * Helpers:   big_a, sacc, seq_is_fixed_width, u128_to_seq, w_add, w_mul,
 *            w_set, w_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <string.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/u128_to_seq.h"
#include "../utils/big_a.h"
#include "../utils/sacc.h"
#include "../utils/w_add.h"
#include "../utils/w_mul.h"
#include "../utils/w_set.h"
#include "../utils/w_sub.h"

static seq_t fix_a(idx_t n) {
    uint64_t c[3], t[3], u[3];
    if (n == 0) {
        w_set(c, 1);
    } else {
        uint64_t y = n + 1;
        uint64_t hp[3], hc[3];
        w_set(hp, 1);
        w_set(hc, y);
        for (idx_t m = 1; m < n; m++) {
            w_mul(t, hc, y);
            w_mul(u, hp, m);
            w_sub(t, t, u);
            memcpy(hp, hc, sizeof hp);
            memcpy(hc, t, sizeof hc);
        }
        w_mul(u, hp, n);
        w_sub(c, hc, u);
    }

    uint64_t b[4][3], nm[3];
    memset(b, 0, sizeof b);
    b[3][0] = 1;
    for (idx_t m = 0; m < n; m++) {
        w_mul(nm, b[3], n);
        if (m >= 1) {
            if (m != 3) {
                w_mul(t, b[2], m);
                w_mul(t, t, m > 3 ? m - 3 : 3 - m);
                if (m > 3) w_add(nm, nm, t); else w_sub(nm, nm, t);
            }
            if (m >= 2) {
                w_mul(t, b[1], n);
                w_mul(t, t, m);
                w_mul(t, t, m - 1);
                w_sub(nm, nm, t);
            }
            if (m >= 3) {
                w_mul(t, b[0], m);
                w_mul(t, t, m - 1);
                w_mul(t, t, m - 2);
                w_add(nm, nm, t);
            }
        }
        memmove(b[0], b[1], 3 * sizeof b[0]);
        memcpy(b[3], nm, sizeof nm);
    }
    w_add(t, b[3], c);
    uint64_t s0 = (t[0] >> 1) | (t[1] << 63);
    uint64_t s1 = (t[1] >> 1) | (t[2] << 63);
    unsigned __int128 v = ((unsigned __int128)s1 << 64) | s0;
    return u128_to_seq(v);
}

seq_t a(idx_t n) {
    if (seq_is_fixed_width()) return fix_a(n);
    return big_a(n);
}
