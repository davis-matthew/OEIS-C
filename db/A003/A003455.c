/*
 * A003455  Number of nonequivalent dissections of an n-gon by nonintersecting
 *          diagonals up to rotation.
 * https://oeis.org/A003455
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/n) * ( A001003(n-2) + Sum_{d|n, d>=2} phi(d) *
 *            A001850(n/d - 1) ), where A001003 are the little Schroeder
 *            numbers (all dissections of an n-gon): (k+1)*s(k) =
 *            3*(2k-1)*s(k-1) - (k-2)*s(k-2), s(0) = s(1) = 1, and A001850 are
 *            the central Delannoy numbers: k*D(k) = 3*(2k-1)*D(k-1) -
 *            (k-1)*D...
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 19, u64 n <= 32, u128 n <= 58,
 *            big any n
 * Helpers:   a3455_big, a3455_is_fixed, a3455_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include "../utils/a3455_big.h"
#include "../utils/a3455_is_fixed.h"
#include "../utils/a3455_phi.h"

#define A3455_L 8

/* ---- fixed-width path: exact 256-bit limb arithmetic ---- */
static void a3455_set(uint32_t *x, uint32_t v) {
    memset(x, 0, sizeof(uint32_t) * A3455_L);
    x[0] = v;
}
static void a3455_mul(uint32_t *dst, const uint32_t *src, uint32_t m) {
    uint64_t c = 0;
    int i;
    for (i = 0; i < A3455_L; i++) {
        uint64_t t = (uint64_t)src[i] * m + c;
        dst[i] = (uint32_t)t;
        c = t >> 32;
    }
}
static void a3455_add(uint32_t *x, const uint32_t *y) {
    uint64_t c = 0;
    int i;
    for (i = 0; i < A3455_L; i++) {
        uint64_t t = (uint64_t)x[i] + y[i] + c;
        x[i] = (uint32_t)t;
        c = t >> 32;
    }
}
static void a3455_sub(uint32_t *x, const uint32_t *y) {
    int64_t b = 0;
    int i;
    for (i = 0; i < A3455_L; i++) {
        int64_t t = (int64_t)x[i] - (int64_t)y[i] - b;
        if (t < 0) { t += ((int64_t)1 << 32); b = 1; } else b = 0;
        x[i] = (uint32_t)t;
    }
}
static void a3455_div(uint32_t *x, uint32_t d) {
    uint64_t r = 0;
    int i;
    for (i = A3455_L - 1; i >= 0; i--) {
        uint64_t cur = (r << 32) | x[i];
        x[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
}

static seq_t a3455_fixed(idx_t n) {
    uint32_t sum[A3455_L], s0[A3455_L], s1[A3455_L], t[A3455_L], u[A3455_L];
    uint32_t d0[A3455_L], d1[A3455_L], cur[A3455_L];
    idx_t k;
    seq_t r = 0;
    int i;
    if (n > 75) return 0; /* value far exceeds any fixed width */
    a3455_set(s0, 1);
    a3455_set(s1, 1);
    for (k = 2; k <= n - 2; k++) {
        a3455_mul(t, s1, (uint32_t)(6 * k - 3));
        a3455_mul(u, s0, (uint32_t)(k - 2));
        a3455_sub(t, u);
        a3455_div(t, (uint32_t)(k + 1));
        memcpy(s0, s1, sizeof(s1));
        memcpy(s1, t, sizeof(t));
    }
    memcpy(sum, s1, sizeof(s1));
    a3455_set(d0, 1);
    a3455_set(d1, 3);
    for (k = 0; k < n / 2; k++) {
        if (k == 0) a3455_set(cur, 1);
        else if (k == 1) a3455_set(cur, 3);
        else {
            a3455_mul(cur, d1, (uint32_t)(6 * k - 3));
            a3455_mul(u, d0, (uint32_t)(k - 1));
            a3455_sub(cur, u);
            a3455_div(cur, (uint32_t)k);
            memcpy(d0, d1, sizeof(d1));
            memcpy(d1, cur, sizeof(cur));
        }
        if (n % (k + 1) == 0 && n / (k + 1) >= 2) {
            a3455_mul(u, cur, (uint32_t)a3455_phi(n / (k + 1)));
            a3455_add(sum, u);
        }
    }
    a3455_div(sum, (uint32_t)n);
    for (i = A3455_L - 1; i >= 0; i--) {
        r = (seq_t)(r * 65536u);
        r = (seq_t)(r * 65536u);
        r = (seq_t)(r + sum[i]);
    }
    return r;
}

seq_t a(idx_t n) {
    if (n < 3) return 0;
    if (a3455_is_fixed()) return a3455_fixed(n);
    return a3455_big(n);
}
