/*
 * A005962  Number of carbon trees with n carbon atoms.
 * https://oeis.org/A005962
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: x*(B(x)^4 + 3*B(x^2)^2 + 8*B(x)*B(x^3))/12, where B(x) = 1
 *            + x*(B(x)^3 + 2*B(x^3))/3 is the g.f. of A000625. Computed as
 *            a(n) = [x^(n-1)] (V(x) + 2*B(x)*B(x^3))/3, with V = P^2 -
 *            P*B(x^2) + B(x^2)^2 and P = (B(x)^2 + B(x^2))/2
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 23, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   alloc_zero, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *bb = 0, *SS = 0, *PP = 0;
static idx_t cnt = 0, cap = 0;

static seq_t mul(seq_t x, seq_t y) {
    return (seq_t)(x * (y + 0u));
}

static seq_t div3(seq_t x) {
    if (seq_is_fixed_width()) {
        seq_t inv = 1;
        int i;
        for (i = 0; i < 8; ++i) {
            seq_t t = (seq_t)((seq_t)2 - mul((seq_t)3, inv));
            inv = mul(inv, t);
        }
        return mul(x, inv);
    }
    return x / 3;
}

static seq_t tri(seq_t x) {
    if (x % 2 == 0) return mul(x / 2, x + 1);
    return mul(x, (x + 1) / 2);
}

static seq_t *regrow(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = alloc_zero(newlen);
    idx_t i;
    for (i = 0; i < oldlen; ++i) p[i] = old[i];
    if (old) release(old, oldlen);
    return p;
}

static void extend(idx_t m) {
    idx_t k, i;
    if (m < cnt) return;
    if (m + 1 > cap) {
        idx_t nc = cap ? cap : 16;
        while (nc < m + 1) nc *= 2;
        bb = regrow(bb, cap, nc);
        SS = regrow(SS, cap, nc);
        PP = regrow(PP, cap, nc);
        cap = nc;
    }
    for (k = cnt; k <= m; ++k) {
        if (k == 0) {
            bb[0] = 1;
        } else {
            seq_t c = 0;
            for (i = 0; i < k; ++i) c += mul(SS[i], bb[k - 1 - i]);
            if ((k - 1) % 3 == 0) c += mul((seq_t)2, bb[(k - 1) / 3]);
            bb[k] = div3(c);
        }
        {
            seq_t s = 0, p = 0;
            for (i = 0; i <= k; ++i) s += mul(bb[i], bb[k - i]);
            for (i = 0; 2 * i < k; ++i) p += mul(bb[i], bb[k - i]);
            if (k % 2 == 0) p += tri(bb[k / 2]);
            SS[k] = s;
            PP[k] = p;
        }
    }
    cnt = m + 1;
}

seq_t a(idx_t n) {
    idx_t m, i;
    seq_t sum1 = 0, sum2 = 0, s3 = 0, t = 0, v;
    if (n == 0) return 0;
    m = n - 1;
    extend(m);
    for (i = 0; i <= m; ++i) sum1 += mul(PP[i], PP[m - i]);
    for (i = 0; 2 * i <= m; ++i) sum2 += mul(bb[i], PP[m - 2 * i]);
    if (m % 2 == 0) s3 = SS[m / 2];
    for (i = 0; 3 * i <= m; ++i) t += mul(bb[i], bb[m - 3 * i]);
    v = sum1 + s3;
    v -= sum2;
    return div3(v + mul((seq_t)2, t));
}
