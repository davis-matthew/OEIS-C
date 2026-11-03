/*
 * A002207  Denominators of logarithmic numbers (also of Gregory coefficients
 *          G(n)).
 * https://oeis.org/A002207
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn, frac, nice
 * Formula:   a(n) = denominator((1/(n+1)!) * Sum_{k=0..n+1}
 *            Stirling1(n+1,k)/(k+1)), a(-1) = 1. With N = n+1, L =
 *            lcm(1..N+1), T = Sum_k Stirling1(N,k)*L/(k+1) (exact multi-limb
 *            integer), a(n) = Prod_{p<=N+1} p^(v_p(L*N!) - min(v_p(T),
 *            v_p(L*N!)))
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 96 (2026-11-01)
 * Helpers:   bit_length_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/bit_length_u64.h"

static void g207_addmul(uint32_t *acc, size_t alen, const uint32_t *x, size_t lx,
                        const uint32_t *y, size_t ly) {
    for (size_t i = 0; i < lx; i++) {
        uint64_t xi = x[i], c = 0;
        size_t j;
        if (xi == 0) continue;
        for (j = 0; j < ly; j++) {
            uint64_t t = xi * y[j] + acc[i + j] + c;
            acc[i + j] = (uint32_t)t;
            c = t >> 32;
        }
        for (j = i + ly; c && j < alen; j++) {
            uint64_t t = (uint64_t)acc[j] + c;
            acc[j] = (uint32_t)t;
            c = t >> 32;
        }
    }
}

static int g207_cmp(const uint32_t *x, size_t lx, const uint32_t *y, size_t ly) {
    if (lx != ly) return lx < ly ? -1 : 1;
    for (size_t i = lx; i-- > 0;)
        if (x[i] != y[i]) return x[i] < y[i] ? -1 : 1;
    return 0;
}

static uint32_t g207_divs(uint32_t *x, size_t len, uint32_t d) {
    uint64_t r = 0;
    for (size_t i = len; i-- > 0;) {
        uint64_t cur = (r << 32) | x[i];
        x[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
    return (uint32_t)r;
}

static uint32_t g207_mods(const uint32_t *x, size_t len, uint32_t d) {
    uint64_t r = 0;
    for (size_t i = len; i-- > 0;) {
        uint64_t cur = (r << 32) | x[i];
        r = cur % d;
    }
    return (uint32_t)r;
}

static uint32_t g207_muls(uint32_t *x, size_t len, uint32_t m) {
    uint64_t c = 0;
    for (size_t i = 0; i < len; i++) {
        uint64_t t = (uint64_t)x[i] * m + c;
        x[i] = (uint32_t)t;
        c = t >> 32;
    }
    return (uint32_t)c;
}

static size_t g207_norm(const uint32_t *x, size_t len) {
    while (len > 0 && x[len - 1] == 0) len--;
    return len;
}

/* a -= b, requires a >= b */
static void g207_sub(uint32_t *x, size_t lx, const uint32_t *y, size_t ly) {
    int64_t br = 0;
    for (size_t i = 0; i < lx; i++) {
        int64_t t = (int64_t)x[i] - (i < ly ? (int64_t)y[i] : 0) - br;
        if (t < 0) { t += ((int64_t)1 << 32); br = 1; } else br = 0;
        x[i] = (uint32_t)t;
    }
}

seq_t a(idx_t n) {
    idx_t N, P, m, k, i, j;
    if (n < 0) return (seq_t)1;
    N = n + 1;
    P = N + 1;

    unsigned char *comp = (unsigned char *)calloc((size_t)P + 1, 1);
    for (i = 2; i * i <= P; i++)
        if (!comp[i])
            for (j = i * i; j <= P; j += i) comp[j] = 1;

    size_t fbits = 1;
    for (m = 1; m <= N; m++) fbits += (size_t)bit_length_u64((uint64_t)m);
    size_t W = fbits / 32 + 2;

    size_t lbits = 1;
    for (i = 2; i <= P; i++) {
        if (comp[i]) continue;
        idx_t pk = i;
        while (pk <= P / i) pk *= i;
        lbits += (size_t)bit_length_u64((uint64_t)pk);
    }
    size_t WL = lbits / 32 + 2;
    uint32_t *L = (uint32_t *)calloc(WL, sizeof(uint32_t));
    size_t lL = 1;
    L[0] = 1;
    for (i = 2; i <= P; i++) {
        if (comp[i]) continue;
        idx_t pk = i;
        while (pk <= P / i) pk *= i;
        uint32_t c = g207_muls(L, lL, (uint32_t)pk);
        if (c) L[lL++] = c;
    }

    /* unsigned Stirling numbers of the first kind, row N */
    uint32_t *c = (uint32_t *)calloc((size_t)(N + 1) * W, sizeof(uint32_t));
    c[W] = 1;
    size_t curbits = 1, cur = 1;
    for (m = 1; m < N; m++) {
        curbits += (size_t)bit_length_u64((uint64_t)(m + 1));
        cur = curbits / 32 + 2;
        if (cur > W) cur = W;
        for (k = m + 1; k >= 1; k--) {
            uint32_t *x = c + (size_t)k * W;
            const uint32_t *y = x - W;
            uint64_t carry = 0;
            for (size_t t = 0; t < cur; t++) {
                uint64_t v = (uint64_t)x[t] * (uint32_t)m + y[t] + carry;
                x[t] = (uint32_t)v;
                carry = v >> 32;
            }
        }
    }

    size_t AW = W + WL + 2;
    uint32_t *pos = (uint32_t *)calloc(AW, sizeof(uint32_t));
    uint32_t *neg = (uint32_t *)calloc(AW, sizeof(uint32_t));
    uint32_t *q = (uint32_t *)calloc(WL, sizeof(uint32_t));
    for (k = 1; k <= N; k++) {
        const uint32_t *ck = c + (size_t)k * W;
        size_t lc = g207_norm(ck, W);
        if (lc == 0) continue;
        memcpy(q, L, lL * sizeof(uint32_t));
        g207_divs(q, lL, (uint32_t)(k + 1));
        size_t lq = g207_norm(q, lL);
        g207_addmul(((N - k) & 1) ? neg : pos, AW, ck, lc, q, lq);
    }

    size_t lp = g207_norm(pos, AW), lm = g207_norm(neg, AW);
    uint32_t *T;
    size_t lT;
    if (g207_cmp(pos, lp, neg, lm) >= 0) {
        g207_sub(pos, lp, neg, lm);
        T = pos;
        lT = g207_norm(pos, lp);
    } else {
        g207_sub(neg, lm, pos, lp);
        T = neg;
        lT = g207_norm(neg, lm);
    }

    seq_t ans = 1;
    for (i = 2; i <= P; i++) {
        if (comp[i]) continue;
        idx_t pk = i, cap = 1, v = 0, e, t;
        while (pk <= P / i) { pk *= i; cap++; }
        for (t = N; t; t /= i) cap += t / i;
        while (v < cap && lT > 0 && g207_mods(T, lT, (uint32_t)i) == 0) {
            g207_divs(T, lT, (uint32_t)i);
            lT = g207_norm(T, lT);
            v++;
        }
        e = cap - v;
        for (t = 0; t < e; t++) ans = ans * (seq_t)i;
    }

    free(comp);
    free(L);
    free(c);
    free(pos);
    free(neg);
    free(q);
    return ans;
}
