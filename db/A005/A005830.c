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
 * Validated: standard, pass, 45 terms through n = 963 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *grow_zero(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    if (p == NULL) return NULL;
    mem_set(p, 0, (size_t)newlen * sizeof(seq_t));
    if (old != NULL) {
        mem_copy(p, old, (size_t)oldlen * sizeof(seq_t));
        mem_free(old);
    }
    return p;
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

/* floor(phi*x) via Zeckendorf representation, no overflow beyond the result */
static seq_t floor_phi_zeck(seq_t x) {
    seq_t F[400];
    int m = 0, j, jmin = -1;
    seq_t r, shift;
    F[0] = 1; /* F_2 */
    m = 1;
    if (x >= 2) {
        F[1] = 2; /* F_3 */
        m = 2;
        while (m < 399 && F[m - 1] <= x - F[m - 2]) {
            F[m] = F[m - 1] + F[m - 2];
            m++;
        }
    }
    r = x;
    shift = 0;
    for (j = m - 1; j >= 0; j--) {
        if (F[j] <= r) {
            r -= F[j];
            if (j == 0) shift += 2;
            else shift += F[j] + F[j - 1];
            jmin = j;
        }
    }
    if (jmin < 0) return 0;
    if (jmin % 2 == 0) shift -= 1; /* k_min = jmin + 2 is even */
    return shift;
}

/* floor(sqrt(N)) by Newton from an upper starting guess g >= floor(sqrt(N)) */
static seq_t isqrt_from(seq_t N, seq_t g) {
    seq_t g1;
    if (g == 0) g = 1;
    for (;;) {
        g1 = (g + N / g) / 2;
        if (g1 >= g) break;
        g = g1;
    }
    return g;
}

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
