/*
 * A007406  Wolstenholme numbers: numerator of Sum_{k=1..n} 1/k^2.
 * https://oeis.org/A007406
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy, nice
 * Formula:   a(n) = numerator of Sum_{k=1..n} 1/k^2, computed exactly as N/D
 *            with D = lcm(1..n)^2, N = Sum_{k=1..n} D/k^2, then removing the
 *            common prime factors p <= n from N (exact multi-limb arithmetic)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 46,
 *            big any n
 * Helpers:   bn_mod_small, prime_power_base
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/bn_mod_small.h"
#include "../utils/prime_power_base.h"

static void bn_mul_small(uint32_t *x, size_t *len, uint32_t m) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < *len; i++) {
        uint64_t t = (uint64_t)x[i] * m + c;
        x[i] = (uint32_t)t;
        c = t >> 32;
    }
    while (c) {
        x[(*len)++] = (uint32_t)c;
        c >>= 32;
    }
}

/* q = x / d, returns remainder; q may alias x */
static uint32_t bn_div_small(uint32_t *q, const uint32_t *x, size_t len, uint32_t d) {
    uint64_t rem = 0;
    size_t i = len;
    while (i > 0) {
        i--;
        uint64_t cur = (rem << 32) | x[i];
        q[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
    return (uint32_t)rem;
}

static void bn_add(uint32_t *x, size_t *xl, const uint32_t *y, size_t yl) {
    uint64_t c = 0;
    size_t i;
    if (*xl < yl) {
        for (i = *xl; i < yl; i++) x[i] = 0;
        *xl = yl;
    }
    for (i = 0; i < *xl; i++) {
        uint64_t t = (uint64_t)x[i] + (i < yl ? y[i] : 0) + c;
        x[i] = (uint32_t)t;
        c = t >> 32;
        if (i >= yl && c == 0) break;
    }
    if (c) x[(*xl)++] = (uint32_t)c;
}

static void bn_trim(const uint32_t *x, size_t *len) {
    while (*len > 1 && x[*len - 1] == 0) (*len)--;
}

seq_t a(idx_t n) {
    uint32_t nn = (uint32_t)n;
    size_t cap = (size_t)nn / 8 + 16;
    uint32_t *N = (uint32_t *)malloc(cap * sizeof(uint32_t));
    uint32_t *D = (uint32_t *)malloc(cap * sizeof(uint32_t));
    uint32_t *Q = (uint32_t *)malloc(cap * sizeof(uint32_t));
    size_t nl = 1, dl = 1, ql, i;
    uint32_t k;
    seq_t r = 0;

    N[0] = 0;
    D[0] = 1;
    for (k = 1; k <= nn; k++) {
        if (k > 1) {
            uint32_t p = prime_power_base(k);
            if (p) {
                bn_mul_small(N, &nl, p * p);
                bn_mul_small(D, &dl, p * p);
            }
        }
        uint32_t k2 = k * k;
        bn_div_small(Q, D, dl, k2);
        ql = dl;
        bn_trim(Q, &ql);
        bn_add(N, &nl, Q, ql);
    }

    /* remove common factors: D's primes are p <= n with exponent 2*floor(log_p n) */
    for (k = 2; k <= nn; k++) {
        if (prime_power_base(k) != k) continue;
        uint32_t e = 0;
        uint64_t pw = k;
        while (pw <= nn) { e += 2; pw *= k; }
        while (e > 0 && bn_mod_small(N, nl, k) == 0) {
            bn_div_small(N, N, nl, k);
            bn_trim(N, &nl);
            e--;
        }
    }

    i = nl;
    while (i > 0) {
        i--;
        uint32_t w = N[i];
        int s;
        for (s = 24; s >= 0; s -= 8) {
            r *= 256;
            r += (w >> s) & 255u;
        }
    }

    free(N);
    free(D);
    free(Q);
    return r;
}
