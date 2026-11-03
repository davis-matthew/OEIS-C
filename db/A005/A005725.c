/*
 * A005725  Quadrinomial coefficients.
 * https://oeis.org/A005725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = [x^n] (1+x+x^2+x^3)^n; equivalently the D-finite
 *            recurrence 2*n*(2*n-1)*(13*n-19)*a(n) = (143*n^3 - 352*n^2 +
 *            251*n - 54)*a(n-1) + 4*(n-1)*(26*n^2 - 51*n + 15)*a(n-2) +
 *            16*(n-2)*(n-1)*(13*n-6)*a(n-3), a(0) = 1, a(1) = 1, a(2) = 3.
 *            Fixed-width builds use the polynomial-power DP (all interme...
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 36, u128 n <= 71,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 190 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static void release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

static bool seq_is_fixed_width(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 256; i++) {
        t *= 2;
        if (t == 0)
            return true;
    }
    return false;
}

static seq_t a_dp(idx_t n) {
    seq_t *c = alloc_zero(n + 1);
    c[0] = 1;
    for (idx_t m = 1; m <= n; m++) {
        for (idx_t j = n; j >= 1; j--) {
            seq_t s = c[j - 1];
            if (j >= 2) s += c[j - 2];
            if (j >= 3) s += c[j - 3];
            c[j] += s;
        }
    }
    seq_t r = c[n];
    release(c, n + 1);
    return r;
}

static seq_t a_rec(idx_t n) {
    if (n == 0 || n == 1) return 1;
    if (n == 2) return 3;
    seq_t a3 = 1, a2 = 1, a1 = 3;
    for (idx_t k = 3; k <= n; k++) {
        seq_t K = (seq_t)k;
        seq_t c1 = ((143 * K - 352) * K + 251) * K - 54;
        seq_t c2 = 4 * (K - 1) * ((26 * K - 51) * K + 15);
        seq_t c3 = 16 * (K - 2) * (K - 1) * (13 * K - 6);
        seq_t d = 2 * K * (2 * K - 1) * (13 * K - 19);
        seq_t v = (c1 * a1 + c2 * a2 + c3 * a3) / d;
        a3 = a2;
        a2 = a1;
        a1 = v;
    }
    return a1;
}

seq_t a(idx_t n) {
    if (seq_is_fixed_width()) return a_dp(n);
    return a_rec(n);
}
