/*
 * A002673  Numerators of central difference coefficients M_{3}^(2n+1).
 * https://oeis.org/A002673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac
 * Formula:   (2*sinh(x/2))^3 = 2*sinh(3x/2) - 6*sinh(x/2), so with k = 2n+1,
 *            M_3^(k) = (3^k - 3)/(2^(k-1)*k!) = 3*(9^n - 1)/(2^(k-1)*k!).
 *            a(n) = numerator = Q / G, where Q = (9^n - 1)/2^v2(9^n - 1) and
 *            G = Product_{5 <= p <= k, p prime} p^min(v_p(9^n - 1), v_p(k!))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 22, u128 n <= 23,
 *            big any n
 * Helpers:   gsum, inv2w, is_fixed_type, pmod9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gsum.h"
#include "../utils/inv2w.h"
#include "../utils/is_fixed_type.h"
#include "../utils/pmod9.h"

seq_t a(idx_t n) {
    idx_t k = 2 * n + 1;
    idx_t m = n;
    idx_t b = 0;
    while ((m & 1) == 0) { m >>= 1; b++; }

    /* Q = (9^n - 1) / 2^(3+b) */
    seq_t Q = gsum((seq_t)3, m);
    Q = Q * ((seq_t)6 * gsum((seq_t)9, (m - 1) / 2) + 1);
    for (idx_t i = 1; i <= b; i++) {
        idx_t e = m << (i - 1);
        Q = Q * ((seq_t)4 * gsum((seq_t)9, e) + 1);
    }

    int fixed = is_fixed_type();
    unsigned char *comp = (unsigned char *)calloc((size_t)k + 1, 1);
    for (idx_t i = 2; i * i <= k; i++)
        if (!comp[i])
            for (idx_t j = i * i; j <= k; j += i) comp[j] = 1;

    for (idx_t p = 5; p <= k; p++) {
        if (comp[p]) continue;
        if (pmod9(n, p) != 1) continue;
        idx_t ep = 0, t = k;
        while (t) { t /= p; ep += t; }
        idx_t v = 0;
        uint64_t q = p;
        while (v < ep) {
            if (pmod9(n, q) != 1) break;
            v++;
            if (q > UINT64_MAX / p) break;
            q *= p;
        }
        if (v == 0) continue;
        if (fixed) {
            seq_t iv = inv2w(p);
            for (idx_t i = 0; i < v; i++) Q = Q * iv;
        } else {
            for (idx_t i = 0; i < v; i++) Q = Q / (seq_t)p;
        }
    }
    free(comp);
    return Q;
}
