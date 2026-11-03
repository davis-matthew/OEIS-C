/*
 * A002682  Denominators of coefficients for repeated integration.
 * https://oeis.org/A002682
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = denominator of ((n+1)/2)*M(n) + (2n+2)*M(n+1), where M(n)
 *            = (2/(2n+1)!) * Integral_{t=0..1} t*Product_{k=1..n} (t^2 - k^2)
 *            = (1/(2n+1)!) * Integral_{u=0..1} Product_{k=1..n} (u - k^2) du.
 *            With F = (n+2)! and g_j = e_j*F/(j+1), where e_j are the
 *            coefficients of Product_{k}(u + k^2), A_m = S...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Validated: partial, pass, 13 terms through n = 12 (2026-11-01)
 * Helpers:   bit_length_u64_to_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/bit_length_u64_to_u64.h"

static void z2682_add(uint32_t *x, const uint32_t *y, size_t L) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t t = (uint64_t)x[i] + y[i] + c;
        x[i] = (uint32_t)t;
        c = t >> 32;
    }
}

static void z2682_sub(uint32_t *x, const uint32_t *y, size_t L) {
    int64_t b = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        int64_t t = (int64_t)x[i] - (int64_t)y[i] - b;
        if (t < 0) { t += ((int64_t)1 << 32); b = 1; } else b = 0;
        x[i] = (uint32_t)t;
    }
}

/* out = Sum_{j=0..k} (-1)^j g_j */
static void z2682_alt(const uint32_t *g, idx_t k, size_t W, uint32_t *out, uint32_t *neg) {
    idx_t j;
    memset(out, 0, W * sizeof(uint32_t));
    memset(neg, 0, W * sizeof(uint32_t));
    for (j = 0; j <= k; j++) {
        if (j % 2 == 0) z2682_add(out, g + (size_t)j * W, W);
        else z2682_add(neg, g + (size_t)j * W, W);
    }
    z2682_sub(out, neg, W);
}

static int z2682_cmp(const uint32_t *x, const uint32_t *y, size_t L) {
    size_t i = L;
    while (i > 0) {
        i--;
        if (x[i] != y[i]) return x[i] < y[i] ? -1 : 1;
    }
    return 0;
}

static void z2682_divs(uint32_t *x, size_t L, uint32_t d) {
    uint64_t r = 0;
    size_t i = L;
    while (i > 0) {
        uint64_t cur;
        i--;
        cur = (r << 32) | x[i];
        x[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
}

static int z2682_iszero(const uint32_t *x, size_t L) {
    size_t i;
    for (i = 0; i < L; i++) if (x[i]) return 0;
    return 1;
}

static uint64_t z2682_leg(uint64_t m, uint64_t p) {
    uint64_t s = 0;
    while (m) { m /= p; s += m; }
    return s;
}

static uint32_t z2682_mods(const uint32_t *x, size_t L, uint32_t d) {
    uint64_t r = 0;
    size_t i = L;
    while (i > 0) {
        i--;
        r = ((r << 32) | x[i]) % d;
    }
    return (uint32_t)r;
}

static void z2682_muls(uint32_t *x, size_t L, uint32_t m) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t t = (uint64_t)x[i] * m + c;
        x[i] = (uint32_t)t;
        c = t >> 32;
    }
}

seq_t a(idx_t n) {
    idx_t m = n + 1, k, j, i;
    size_t bits = 256, fbits = 64, W, cur;
    uint32_t *g, *tmp, *An, *An1, *X;
    uint64_t lim, p;
    unsigned char *comp;
    seq_t den = 1;

    for (k = 1; k <= m; k++) bits += 2 * bit_length_u64_to_u64(k) + 1;
    for (i = 1; i <= n + 2; i++) { bits += bit_length_u64_to_u64(i); fbits += bit_length_u64_to_u64(i); }
    W = bits / 32 + 2;

    g = (uint32_t *)calloc((size_t)(m + 1) * W, sizeof(uint32_t));
    tmp = (uint32_t *)calloc(W, sizeof(uint32_t));
    An = (uint32_t *)calloc(W, sizeof(uint32_t));
    An1 = (uint32_t *)calloc(W, sizeof(uint32_t));
    X = (uint32_t *)calloc(W, sizeof(uint32_t));

    /* g_0 = F = (n+2)! */
    g[0] = 1;
    for (i = 2; i <= n + 2; i++) z2682_muls(g, W, (uint32_t)i);
    cur = fbits / 32 + 2;
    if (cur > W) cur = W;

    for (k = 0; k <= m; k++) {
        if (k > 0) {
            fbits += 2 * bit_length_u64_to_u64(k) + 1;
            cur = fbits / 32 + 2;
            if (cur > W) cur = W;
            for (j = k; j >= 1; j--) {
                uint32_t *gj = g + (size_t)j * W;
                memcpy(tmp, g + (size_t)(j - 1) * W, cur * sizeof(uint32_t));
                z2682_muls(tmp, cur, (uint32_t)j);
                z2682_divs(tmp, cur, (uint32_t)(j + 1));
                z2682_muls(gj, cur, (uint32_t)k);
                z2682_muls(gj, cur, (uint32_t)k);
                z2682_add(gj, tmp, cur);
            }
            z2682_muls(g, cur, (uint32_t)k);
            z2682_muls(g, cur, (uint32_t)k);
        }
        if (k == n) z2682_alt(g, k, W, An, tmp);
        if (k == m) z2682_alt(g, k, W, An1, tmp);
    }

    /* X = |(2n+2)(2n+3) A_n - 4 A_{n+1}| */
    z2682_muls(An, W, (uint32_t)(2 * n + 2));
    z2682_muls(An, W, (uint32_t)(2 * n + 3));
    z2682_muls(An1, W, 4);
    if (z2682_cmp(An, An1, W) >= 0) {
        memcpy(X, An, W * sizeof(uint32_t));
        z2682_sub(X, An1, W);
    } else {
        memcpy(X, An1, W * sizeof(uint32_t));
        z2682_sub(X, An, W);
    }

    if (!z2682_iszero(X, W)) {
        lim = 2 * (uint64_t)n + 3;
        comp = (unsigned char *)calloc((size_t)lim + 1, 1);
        for (p = 2; p <= lim; p++) {
            uint64_t vD, vN = 0, e;
            if (comp[p]) continue;
            for (e = p * p; e <= lim; e += p) comp[e] = 1;
            vD = (p == 2 ? 1 : 0) + z2682_leg(lim, p) + z2682_leg((uint64_t)n + 2, p);
            {
                uint64_t t = (uint64_t)n + 1;
                while (t % p == 0) { vN++; t /= p; }
            }
            if (vN < vD) {
                memcpy(tmp, X, W * sizeof(uint32_t));
                while (vN < vD && z2682_mods(tmp, W, (uint32_t)p) == 0) {
                    z2682_divs(tmp, W, (uint32_t)p);
                    vN++;
                }
            }
            for (e = vN; e < vD; e++) den *= (seq_t)p;
        }
        free(comp);
    }

    free(g); free(tmp); free(An); free(An1); free(X);
    return den;
}
