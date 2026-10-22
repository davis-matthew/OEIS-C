/*
 * A007137  Number of rooted maps with n edges on the projective plane.
 * https://oeis.org/A007137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   D-finite recurrence derived from the OEIS ODE: (m+2)(m+3)(m+4)
 *            a(m+3) = 1728 m(2m-1)(2m+1) a(m-1) - 216(2m+1)(4m+3) a(m) -
 *            144(m+1)(m+2)(2m+1) a(m+1) + 4(m+2)(m+3)(8m+11) a(m+2), a(-1) =
 *            a(0) = 0, a(1) = 1, a(2) = 10. For fixed-width builds (where the
 *            division would not be exact modulo 2^k), the...
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 37,
 *            big any n
 * Helpers:   alloc_zero, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_fixed_width.h"

static seq_t a_fixed(idx_t n) {
    seq_t *y = alloc_zero(n + 1);
    seq_t *y2 = alloc_zero(n + 1);
    seq_t *y3 = alloc_zero(n + 1);
    seq_t *y4 = alloc_zero(n + 1);
    seq_t r;
    for (idx_t m = 1; m <= n; m++) {
        seq_t v = 0;
        if (m == 1) v += 1;
        v += y[m - 1] * 10;
        v -= y2[m - 1] * 2;
        if (m >= 2) {
            v += y2[m - 2] * 42;
            v -= y3[m - 2] * 6;
        }
        if (m >= 3) v += y4[m - 3] * 9;
        y[m] = v;
        {
            seq_t s2 = 0, s3 = 0, s4 = 0;
            for (idx_t i = 1; i < m; i++) {
                s2 += y[i] * y[m - i];
                s3 += y2[i] * y[m - i];
                s4 += y3[i] * y[m - i];
            }
            y2[m] = s2;
            y3[m] = s3;
            y4[m] = s4;
        }
    }
    r = y[n];
    release(y, n + 1);
    release(y2, n + 1);
    release(y3, n + 1);
    release(y4, n + 1);
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (n == 2) return 10;
    if (seq_is_fixed_width()) return a_fixed(n);
    {
        seq_t am1 = 0, am = 0, ap1 = 1, ap2 = 10;
        for (idx_t m = 0; m + 3 <= n; m++) {
            seq_t M = (seq_t)m;
            seq_t pos = M * 1728 * (M * 2 + 1);
            seq_t neg;
            seq_t next;
            if (m >= 1) pos = pos * (M * 2 - 1) * am1;
            else pos = 0;
            pos += (M + 2) * (M + 3) * (M * 8 + 11) * 4 * ap2;
            neg = (M * 2 + 1) * (M * 4 + 3) * 216 * am;
            neg += (M + 1) * (M + 2) * (M * 2 + 1) * 144 * ap1;
            next = pos - neg;
            next /= (M + 2) * (M + 3) * (M + 4);
            am1 = am;
            am = ap1;
            ap1 = ap2;
            ap2 = next;
        }
        return ap2;
    }
}
