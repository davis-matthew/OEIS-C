/*
 * A005181  a(n) = ceiling(exp((n-1)/2)).
 * https://oeis.org/A005181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(exp((n-1)/2)); exact integer interval arithmetic:
 *            sqrt(e) bounded via its Taylor series in fixed point, raised to
 *            the (n-1)th power, precision increased until floor bounds agree
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 45, u64 n <= 89, u128 n <= 178,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>

static size_t bn_norm(const uint32_t *x, size_t len) {
    while (len > 0 && x[len - 1] == 0) len--;
    return len;
}

/* r = r + a (r buffer must have room for one extra limb) */
static size_t bn_add(uint32_t *r, size_t rl, const uint32_t *x, size_t xl) {
    size_t n = rl > xl ? rl : xl, i;
    uint64_t c = 0;
    for (i = 0; i < n; i++) {
        uint64_t s = (uint64_t)(i < rl ? r[i] : 0) + (i < xl ? x[i] : 0) + c;
        r[i] = (uint32_t)s;
        c = s >> 32;
    }
    if (c) r[n++] = (uint32_t)c;
    return n;
}

/* t = floor(t / d) */
static size_t bn_divsmall(uint32_t *t, size_t tl, uint32_t d) {
    uint64_t rem = 0;
    size_t i = tl;
    while (i > 0) {
        i--;
        uint64_t cur = (rem << 32) | t[i];
        t[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
    return bn_norm(t, tl);
}

/* r = floor(a*b / 2^(32*sh)) (+1 if up); r may alias a or b */
static size_t bn_mulshift(uint32_t *r, const uint32_t *x, size_t xl,
                          const uint32_t *y, size_t yl, size_t sh, int up,
                          uint32_t *tmp) {
    size_t tl = xl + yl, i, j, rl;
    memset(tmp, 0, (tl + 1) * sizeof(uint32_t));
    for (i = 0; i < xl; i++) {
        uint64_t c = 0, xi = x[i];
        for (j = 0; j < yl; j++) {
            uint64_t cur = xi * y[j] + tmp[i + j] + c;
            tmp[i + j] = (uint32_t)cur;
            c = cur >> 32;
        }
        tmp[i + yl] = (uint32_t)c;
    }
    rl = tl > sh ? tl - sh : 0;
    if (rl) memmove(r, tmp + sh, rl * sizeof(uint32_t));
    r[rl] = 0;
    if (up) {
        i = 0;
        while (++r[i] == 0) i++;
    }
    return bn_norm(r, rl + 1);
}

/* res = base^e in fixed point with sh fractional limbs; base is destroyed */
static size_t bn_powfix(uint32_t *res, uint32_t *base, size_t bl, uint64_t e,
                        size_t sh, int up, uint32_t *tmp, size_t cap) {
    size_t rl;
    memset(res, 0, cap * sizeof(uint32_t));
    res[sh] = 1;
    rl = sh + 1;
    while (e) {
        if (e & 1) rl = bn_mulshift(res, res, rl, base, bl, sh, up, tmp);
        e >>= 1;
        if (e) bl = bn_mulshift(base, base, bl, base, bl, sh, up, tmp);
    }
    return rl;
}

seq_t a(idx_t n) {
    uint64_t m;
    size_t intl, pl;
    if (n <= 1) return 1;
    m = (uint64_t)n - 1;
    intl = (size_t)((m * 3 / 4) / 32 + 2);
    pl = intl + 3;
    for (;;) {
        size_t cap = pl + intl + 4;
        uint32_t *lo = (uint32_t *)calloc(cap, sizeof(uint32_t));
        uint32_t *hi = (uint32_t *)calloc(cap, sizeof(uint32_t));
        uint32_t *t = (uint32_t *)calloc(cap, sizeof(uint32_t));
        uint32_t *rlo = (uint32_t *)calloc(cap, sizeof(uint32_t));
        uint32_t *rhi = (uint32_t *)calloc(cap, sizeof(uint32_t));
        uint32_t *tmp = (uint32_t *)calloc(2 * cap + 2, sizeof(uint32_t));
        size_t tl, lol, hil, rll, rhl, i;
        uint32_t k, small[2];
        bool same;

        /* sqrt(e) = sum 1/(2^k k!) : lower bound by truncated terms */
        t[pl] = 1;
        tl = pl + 1;
        lo[pl] = 1;
        lol = pl + 1;
        k = 1;
        for (;;) {
            tl = bn_divsmall(t, tl, 2 * k);
            if (tl == 0) break;
            lol = bn_add(lo, lol, t, tl);
            k++;
        }
        memcpy(hi, lo, lol * sizeof(uint32_t));
        small[0] = 2 * k + 10;
        small[1] = 0;
        hil = bn_add(hi, lol, small, 1);

        rll = bn_powfix(rlo, lo, lol, m, pl, 0, tmp, cap);
        rhl = bn_powfix(rhi, hi, hil, m, pl, 1, tmp, cap);

        same = (rll == rhl);
        if (same) {
            for (i = pl; i < rll; i++)
                if (rlo[i] != rhi[i]) { same = false; break; }
        }
        if (same) {
            seq_t r = 0;
            bool started = false;
            size_t li = rll;
            while (li > pl) {
                int s;
                li--;
                for (s = 24; s >= 0; s -= 8) {
                    uint32_t byte = (rlo[li] >> s) & 255u;
                    if (!started && byte == 0) continue;
                    started = true;
                    r = r * 256 + (idx_t)byte;
                }
            }
            free(lo); free(hi); free(t); free(rlo); free(rhi); free(tmp);
            r += 1; /* e^((n-1)/2) is irrational for n > 1 */
            return r;
        }
        free(lo); free(hi); free(t); free(rlo); free(rhi); free(tmp);
        pl += 2;
    }
}
