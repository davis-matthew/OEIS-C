/*
 * A003508  a(1) = 1; for n>1, a(n) = a(n-1) + 1 + sum of distinct prime
 *          factors of a(n-1) that are < a(n-1).
 * https://oeis.org/A003508
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(1) = 1; a(n) = a(n-1) + 1 + (sum of distinct primes p | a(n-1)
 *            with p < a(n-1)). Computed iteratively; a(n-1) is factored by
 *            trial division + Miller-Rabin + Pollard-Brent rho
 * Range:     u8 n <= 26, u16 n <= 145, u32 n <= 406, u64 n <= 1833,
 *            u128 n <= 2000, big any n
 * Helpers:   s3508_fits, s3508_gcdS, s3508_pwS, s3508_rhobig
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/s3508_fits.h"
#include "../utils/s3508_gcdS.h"
#include "../utils/s3508_pwS.h"
#include "../utils/s3508_rhobig.h"

typedef unsigned __int128 s3508_u128;

static const idx_t s3508_SP[46] = {
    2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53,59,61,67,71,73,79,83,89,97,
    101,103,107,109,113,127,131,137,139,149,151,157,163,167,173,179,181,191,193,197,199
};

static s3508_u128 s3508_to128(seq_t x) {
    s3508_u128 r = 0;
    int i;
    for (i = 0; i < 8; i++) {
        unsigned long long c = (unsigned long long)(long long)(x % 65536);
        r |= (s3508_u128)c << (16 * i);
        x /= 65536;
    }
    return r;
}

static seq_t s3508_from128(s3508_u128 v) {
    seq_t r = 0;
    int i;
    for (i = 7; i >= 0; i--) {
        r = r * (idx_t)65536 + (idx_t)((unsigned long long)(v >> (16 * i)) & 0xFFFFULL);
    }
    return r;
}

/* ---------- 128-bit Montgomery arithmetic ---------- */
typedef struct { s3508_u128 n, ni, one, r2; } s3508_mctx;

static void s3508_mul(s3508_u128 a, s3508_u128 b, s3508_u128 *hi, s3508_u128 *lo) {
    uint64_t a0 = (uint64_t)a, a1 = (uint64_t)(a >> 64);
    uint64_t b0 = (uint64_t)b, b1 = (uint64_t)(b >> 64);
    s3508_u128 p00 = (s3508_u128)a0 * b0, p01 = (s3508_u128)a0 * b1;
    s3508_u128 p10 = (s3508_u128)a1 * b0, p11 = (s3508_u128)a1 * b1;
    s3508_u128 mid = (p00 >> 64) + (uint64_t)p01 + (uint64_t)p10;
    *lo = (mid << 64) | (uint64_t)p00;
    *hi = p11 + (p01 >> 64) + (p10 >> 64) + (mid >> 64);
}

static s3508_u128 s3508_addmod(s3508_u128 a, s3508_u128 b, s3508_u128 n) {
    s3508_u128 s = a + b;
    if (s < a || s >= n) s -= n;
    return s;
}

static s3508_u128 s3508_mmul(const s3508_mctx *c, s3508_u128 a, s3508_u128 b) {
    s3508_u128 hi, lo, mh, ml, m, t1, t2;
    int ov;
    s3508_mul(a, b, &hi, &lo);
    m = lo * c->ni;
    s3508_mul(m, c->n, &mh, &ml);
    t1 = hi + mh;
    ov = t1 < hi;
    t2 = t1 + (lo != 0 ? 1 : 0);
    if (t2 < t1) ov = 1;
    if (ov || t2 >= c->n) t2 -= c->n;
    return t2;
}

static void s3508_mset(s3508_mctx *c, s3508_u128 n) {
    s3508_u128 x = n, r;
    int i;
    for (i = 0; i < 7; i++) x *= (s3508_u128)2 - n * x;
    c->n = n;
    c->ni = (s3508_u128)0 - x;
    c->one = ((s3508_u128)0 - n) % n;
    r = c->one;
    for (i = 0; i < 128; i++) r = s3508_addmod(r, r, n);
    c->r2 = r;
}

static s3508_u128 s3508_toM(const s3508_mctx *c, s3508_u128 a) {
    return s3508_mmul(c, a % c->n, c->r2);
}

static s3508_u128 s3508_mpow(const s3508_mctx *c, s3508_u128 b, s3508_u128 e) {
    s3508_u128 r = c->one;
    while (e) {
        if (e & 1) r = s3508_mmul(c, r, b);
        b = s3508_mmul(c, b, b);
        e >>= 1;
    }
    return r;
}

static s3508_u128 s3508_gcd128(s3508_u128 a, s3508_u128 b) {
    while (b) { s3508_u128 t = a % b; a = b; b = t; }
    return a;
}

static int s3508_mr128(s3508_u128 n) {
    int i, s = 0, j;
    s3508_u128 d;
    s3508_mctx c;
    if (n < 2) return 0;
    for (i = 0; i < 25; i++) {
        if (n == s3508_SP[i]) return 1;
        if (n % s3508_SP[i] == 0) return 0;
    }
    if (n < 9409) return 1;
    d = n - 1;
    while ((d & 1) == 0) { d >>= 1; s++; }
    s3508_mset(&c, n);
    {
        s3508_u128 mone = n - c.one;
        for (i = 0; i < 20; i++) {
            s3508_u128 x = s3508_mpow(&c, s3508_toM(&c, s3508_SP[i]), d);
            if (x == c.one || x == mone) continue;
            for (j = 1; j < s; j++) {
                x = s3508_mmul(&c, x, x);
                if (x == mone) break;
            }
            if (j >= s) return 0;
        }
    }
    return 1;
}

static s3508_u128 s3508_f128(const s3508_mctx *c, s3508_u128 y, s3508_u128 cc) {
    return s3508_addmod(s3508_mmul(c, y, y), cc, c->n);
}

static s3508_u128 s3508_rho128(s3508_u128 n) {
    s3508_mctx c;
    s3508_u128 cc;
    const unsigned long M = 128;
    s3508_mset(&c, n);
    for (cc = 1;; cc++) {
        s3508_u128 y = (cc + 1) % n, x = 0, ys = 0, q = c.one, g = 1;
        unsigned long r = 1, k, i, lim;
        do {
            x = y;
            for (i = 0; i < r; i++) y = s3508_f128(&c, y, cc);
            k = 0;
            do {
                ys = y;
                lim = (M < r - k) ? M : r - k;
                for (i = 0; i < lim; i++) {
                    y = s3508_f128(&c, y, cc);
                    q = s3508_mmul(&c, q, x > y ? x - y : y - x);
                }
                g = s3508_gcd128(q, n);
                k += M;
            } while (k < r && g == 1);
            r <<= 1;
        } while (g == 1);
        if (g == n) {
            do {
                ys = s3508_f128(&c, ys, cc);
                g = s3508_gcd128(x > ys ? x - ys : ys - x, n);
            } while (g == 1);
        }
        if (g != n) return g;
    }
}

static int s3508_mrbig(seq_t n) {
    seq_t d = n - 1, nm1 = n - 1;
    int s = 0, i, j;
    while (d % 2 == 0) { d /= 2; s++; }
    for (i = 0; i < 20; i++) {
        seq_t a0 = 0;
        seq_t x;
        a0 += s3508_SP[i];
        x = s3508_pwS(a0, d, n);
        if (x == 1 || x == nm1) continue;
        for (j = 1; j < s; j++) {
            x = x * x % n;
            if (x == nm1) break;
        }
        if (j >= s) return 0;
    }
    return 1;
}

/* ---------- sum of distinct prime factors of m that are < m ---------- */
static seq_t s3508_sopf(seq_t m) {
    seq_t n = m, sum = 0;
    seq_t st[256];
    seq_t pr[256];
    int sp = 0, np = 0, i, j;
    if (m <= 1) return 0;
    for (i = 0; i < 46; i++) {
        idx_t p = s3508_SP[i];
        if (n % p == 0) {
            if (m > p) sum += p;
            do { n /= p; } while (n % p == 0);
        }
    }
    if (n == 1) return sum;
    st[sp++] = n;
    while (sp > 0) {
        seq_t x = st[--sp];
        int fit, isp;
        seq_t d;
        if (x == 1) continue;
        fit = s3508_fits(x);
        isp = fit ? s3508_mr128(s3508_to128(x)) : s3508_mrbig(x);
        if (isp) {
            for (j = 0; j < np; j++) if (pr[j] == x) break;
            if (j == np && np < 256) pr[np++] = x;
            continue;
        }
        d = fit ? s3508_from128(s3508_rho128(s3508_to128(x))) : s3508_rhobig(x);
        st[sp++] = d;
        st[sp++] = x / d;
    }
    for (j = 0; j < np; j++) if (pr[j] < m) sum += pr[j];
    return sum;
}

static seq_t *s3508_cache = NULL;
static idx_t s3508_cap = 0;
static idx_t s3508_done = 0;

seq_t a(idx_t n) {
    idx_t i;
    if (n == 0) return 0;
    if (n == 1) return 1;
    if (s3508_cache == NULL) {
        s3508_cap = (n >= 32) ? n + 32 : 64;
        s3508_cache = (seq_t *)malloc(s3508_cap * sizeof(seq_t));
        s3508_cache[1] = 1;
        s3508_done = 1;
    }
    while (n >= s3508_cap) {
        idx_t nc = s3508_cap * 2;
        s3508_cache = (seq_t *)realloc(s3508_cache, nc * sizeof(seq_t));
        s3508_cap = nc;
    }
    if (n <= s3508_done) return s3508_cache[n];
    for (i = s3508_done + 1; i <= n; i++) {
        seq_t prev = s3508_cache[i - 1];
        s3508_cache[i] = prev + 1 + s3508_sopf(prev);
    }
    s3508_done = n;
    return s3508_cache[n];
}
