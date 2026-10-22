/*
 * A005957  Number of acyclic ketone and aldehyde stereo-isomers with n carbon
 *          atoms.
 * https://oeis.org/A005957
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^(n-1)] (B(x)^2 + B(x^2))/2, where B(x) = Sum b(k) x^k
 *            is the g.f. of A000625, B(x) = 1 + x*(B(x)^3 + 2*B(x^3))/3, b(0)
 *            = 1
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 24, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   grow_zero, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_zero.h"
#include "../utils/seq_is_fixed_width.h"

static seq_t *bb = NULL;   /* A000625 coefficients */
static seq_t *cc = NULL;   /* coefficients of B(x)^2 */
static idx_t have = 0, cap = 0;

static seq_t div3(seq_t s) {
    if (seq_is_fixed_width()) {
        seq_t x = 3;
        int it;
        for (it = 0; it < 8; it++) {
            seq_t t = (seq_t)(3 * x);
            seq_t u = (seq_t)(2 - t);
            x = (seq_t)(x * u);
        }
        return (seq_t)(s * x);
    }
    return s / 3;
}

static void ensure(idx_t need) {
    if (cap < need) {
        idx_t nc = cap * 2;
        if (nc < need) nc = need;
        if (nc < 16) nc = 16;
        bb = grow_zero(bb, cap, nc);
        cc = grow_zero(cc, cap, nc);
        cap = nc;
    }
    if (have == 0) {
        bb[0] = 1;
        have = 1;
    }
    while (have < need) {
        idx_t k = have - 1, i;
        seq_t s = 0;
        for (i = 0; i <= k; i++) s += bb[i] * bb[k - i];
        cc[k] = s;
        s = 0;
        for (i = 0; i <= k; i++) s += cc[i] * bb[k - i];
        if (k % 3 == 0) s += 2 * bb[k / 3];
        bb[k + 1] = div3(s);
        have++;
    }
}

seq_t a(idx_t n) {
    idx_t m, i;
    seq_t res = 0;
    if (n < 1) return 0;
    m = n - 1;
    ensure(m + 1);
    for (i = 0; i < m - i; i++) res += bb[i] * bb[m - i];
    if (m % 2 == 0) {
        seq_t h = bb[m / 2];
        if (h % 2 == 0) res += (h / 2) * (h + 1);
        else res += h * ((h + 1) / 2);
    }
    return res;
}
