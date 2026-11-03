/*
 * A001674  a(n) = floor(sqrt( 2*Pi )^n).
 * https://oeis.org/A001674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((2*Pi)^(n/2)), computed exactly with multi-limb
 *            fixed-point arithmetic: Pi = 16*atan(1/5) - 4*atan(1/239)
 *            (Machin), sqrt(2*Pi) by digit-by-digit integer square root,
 *            (2*Pi)^floor(n/2) by binary powering, times sqrt(2*Pi) when n is
 *            odd
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 96,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 455 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void a1674_add(uint32_t *a, const uint32_t *b, size_t len) {
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < len; i++) {
        uint64_t cur = (uint64_t)a[i] + b[i] + carry;
        a[i] = (uint32_t)cur;
        carry = cur >> 32;
    }
}

static void a1674_divs(uint32_t *x, size_t len, uint32_t d) {
    uint64_t rem = 0;
    size_t i = len;
    while (i > 0) {
        i--;
        uint64_t cur = (rem << 32) | x[i];
        x[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
}

static int a1674_iszero(const uint32_t *x, size_t len) {
    size_t i;
    for (i = 0; i < len; i++) if (x[i]) return 0;
    return 1;
}

static void a1674_sub(uint32_t *a, const uint32_t *b, size_t len) {
    int64_t borrow = 0;
    size_t i;
    for (i = 0; i < len; i++) {
        int64_t cur = (int64_t)a[i] - (int64_t)b[i] - borrow;
        if (cur < 0) { cur += (int64_t)1 << 32; borrow = 1; } else borrow = 0;
        a[i] = (uint32_t)cur;
    }
}

/* out = atan(1/x) in fixed point */
static void a1674_atan_inv(uint32_t *out, uint32_t x, size_t L, size_t F,
                           uint32_t *t, uint32_t *s) {
    uint32_t k = 0;
    memset(out, 0, L * sizeof(uint32_t));
    memset(t, 0, L * sizeof(uint32_t));
    t[F] = 1;
    a1674_divs(t, L, x);
    while (!a1674_iszero(t, L)) {
        memcpy(s, t, L * sizeof(uint32_t));
        a1674_divs(s, L, 2 * k + 1);
        if (k & 1) a1674_sub(out, s, L); else a1674_add(out, s, L);
        a1674_divs(t, L, x);
        a1674_divs(t, L, x);
        k++;
    }
}

static int a1674_cmp(const uint32_t *a, const uint32_t *b, size_t len) {
    size_t i = len;
    while (i > 0) {
        i--;
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    }
    return 0;
}

/* out = a*b in fixed point with F fractional limbs, length L */
static void a1674_mulfix(uint32_t *out, const uint32_t *a, const uint32_t *b,
                         size_t L, size_t F, uint32_t *t) {
    size_t i, j;
    memset(t, 0, 2 * L * sizeof(uint32_t));
    for (i = 0; i < L; i++) {
        uint64_t carry = 0;
        if (a[i] == 0) continue;
        for (j = 0; j < L; j++) {
            uint64_t cur = (uint64_t)a[i] * b[j] + t[i + j] + carry;
            t[i + j] = (uint32_t)cur;
            carry = cur >> 32;
        }
        t[i + L] = (uint32_t)carry;
    }
    for (j = 0; j < L; j++) out[j] = t[F + j];
}

static void a1674_muls(uint32_t *x, size_t len, uint32_t m) {
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < len; i++) {
        uint64_t cur = (uint64_t)x[i] * m + carry;
        x[i] = (uint32_t)cur;
        carry = cur >> 32;
    }
}

static void a1674_shl(uint32_t *x, size_t len, unsigned s) {
    size_t i;
    for (i = len - 1; i > 0; i--)
        x[i] = (x[i] << s) | (x[i - 1] >> (32 - s));
    x[0] <<= s;
}

seq_t a(idx_t n) {
    uint64_t bitsInt = (uint64_t)n * 1326 / 1000 + 8;
    size_t I = (size_t)(bitsInt / 32 + 2);
    size_t F = (size_t)(bitsInt / 32 + 5);
    size_t L = I + F;
    size_t R = F + 3, NL = 2 * F + 1;
    uint32_t *c = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *b2 = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(2 * L, sizeof(uint32_t));
    uint32_t *s = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *res = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *base = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *tmp = (uint32_t *)calloc(L, sizeof(uint32_t));
    idx_t k = n / 2, e;
    seq_t r = 0;
    size_t i;

    /* c = 2*Pi = 32*atan(1/5) - 8*atan(1/239) */
    a1674_atan_inv(c, 5, L, F, t, s);
    a1674_atan_inv(b2, 239, L, F, t, s);
    a1674_muls(c, L, 32);
    a1674_muls(b2, L, 8);
    a1674_sub(c, b2, L);

    if (n & 1) {
        /* sqrt(2*Pi) in fixed point: isqrt(c * 2^P) */
        uint32_t *N = (uint32_t *)calloc(NL, sizeof(uint32_t));
        uint32_t *rem = (uint32_t *)calloc(R, sizeof(uint32_t));
        uint32_t *root = (uint32_t *)calloc(R, sizeof(uint32_t));
        uint32_t *trial = (uint32_t *)calloc(R, sizeof(uint32_t));
        size_t li;
        for (i = 0; i <= F; i++) N[i + F] = c[i];
        li = NL;
        while (li > 0) {
            int sh;
            li--;
            for (sh = 30; sh >= 0; sh -= 2) {
                uint32_t pair = (N[li] >> sh) & 3u;
                a1674_shl(rem, R, 2);
                rem[0] |= pair;
                memcpy(trial, root, R * sizeof(uint32_t));
                a1674_shl(trial, R, 2);
                trial[0] |= 1u;
                a1674_shl(root, R, 1);
                if (a1674_cmp(rem, trial, R) >= 0) {
                    a1674_sub(rem, trial, R);
                    root[0] |= 1u;
                }
            }
        }
        memset(s, 0, L * sizeof(uint32_t));
        for (i = 0; i < R && i < L; i++) s[i] = root[i];
        free(N); free(rem); free(root); free(trial);
    }

    /* res = c^k */
    memset(res, 0, L * sizeof(uint32_t));
    res[F] = 1;
    memcpy(base, c, L * sizeof(uint32_t));
    e = k;
    while (e) {
        if (e & 1) {
            a1674_mulfix(tmp, res, base, L, F, t);
            memcpy(res, tmp, L * sizeof(uint32_t));
        }
        e >>= 1;
        if (e) {
            a1674_mulfix(tmp, base, base, L, F, t);
            memcpy(base, tmp, L * sizeof(uint32_t));
        }
    }
    if (n & 1) {
        a1674_mulfix(tmp, res, s, L, F, t);
        memcpy(res, tmp, L * sizeof(uint32_t));
    }

    i = L;
    while (i > F) {
        i--;
        r = r * (seq_t)65536;
        r = r * (seq_t)65536;
        r = r + (seq_t)res[i];
    }

    free(c); free(b2); free(t); free(s); free(res); free(base); free(tmp);
    return r;
}
