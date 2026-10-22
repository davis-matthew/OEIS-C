/*
 * A005630  Number of chiral trees with n nodes.
 * https://oeis.org/A005630
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A000628(n) - A005629(n), by Polya counting of steric
 *            alkane trees: T(x) = 1 + x*(T(x)^3 + 2*T(x^3))/3, T(0) = 1
 *            (steric planted trees), A(x) = 1 + x*A(x)*T(x^2), A(0) = 1
 *            (achiral planted trees), 12*a(n) = [x^(n-1)] (T^4 + 3*T(x^2)^2 +
 *            8*T*T(x^3) - 6*T(x^2)*A^2 - 6*T(x^4)) - 6*[x^n] ((T-1)...
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 26, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   alloc_zero, release, seq_is_fixed_width, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/u128_to_seq.h"

typedef unsigned __int128 u128;

#define PR 4294967291ULL

static u128 rd(u128 x, uint64_t p) { return p ? x % p : x; }

static u128 sb(u128 x, u128 y, uint64_t p)
{
    return p ? (x % p + p - y % p) % p : x - y;
}

static uint64_t powm(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t r = 1;
    b %= m;
    while (e) {
        if (e & 1) r = (uint64_t)((u128)r * b % m);
        b = (uint64_t)((u128)b * b % m);
        e >>= 1;
    }
    return r;
}

static u128 inv3_128(void)
{
    u128 x = 3;
    int i;
    for (i = 0; i < 7; i++) x *= (u128)2 - (u128)3 * x;
    return x;
}

/* out[n] = 12*a(n) modulo p (or modulo 2^128 if p == 0), n = 1..L */
static void series_fixed(idx_t L, uint64_t p, u128 *out)
{
    size_t sz = (size_t)(L + 1) * sizeof(u128);
    u128 *t = (u128 *)malloc(sz), *ac = (u128 *)malloc(sz);
    u128 *c = (u128 *)malloc(sz), *e = (u128 *)malloc(sz), *d = (u128 *)malloc(sz);
    u128 inv3 = p ? (u128)powm(3, p - 2, p) : inv3_128();
    idx_t k, i, n;
    for (k = 0; k <= L; k++) {
        u128 s;
        if (k == 0) {
            t[0] = 1;
            ac[0] = 1;
        } else {
            s = e[k - 1];
            if ((k - 1) % 3 == 0) s += 2 * t[(k - 1) / 3];
            t[k] = rd(rd(s, p) * inv3, p);
            s = 0;
            for (i = 0; 2 * i <= k - 1; i++) s += t[i] * ac[k - 1 - 2 * i];
            ac[k] = rd(s, p);
        }
        s = 0;
        for (i = 0; i <= k; i++) s += t[i] * t[k - i];
        c[k] = rd(s, p);
        s = 0;
        for (i = 0; i <= k; i++) s += c[i] * t[k - i];
        e[k] = rd(s, p);
        s = 0;
        for (i = 0; i <= k; i++) s += ac[i] * ac[k - i];
        d[k] = rd(s, p);
    }
    for (n = 1; n <= L; n++) {
        idx_t m = n - 1;
        u128 q = 0, x = 0, y = 0, pos, neg;
        for (i = 0; i <= m; i++) q += c[i] * c[m - i];
        for (i = 0; 3 * i <= m; i++) x += t[i] * t[m - 3 * i];
        for (i = 0; 2 * i <= m; i++) y += t[i] * d[m - 2 * i];
        pos = rd(q, p) + 8 * rd(x, p) + 6 * sb(d[n], 2 * ac[n], p);
        if (m % 2 == 0) pos += 3 * c[m / 2];
        neg = 6 * rd(y, p) + 6 * sb(c[n], 2 * t[n], p);
        if (m % 4 == 0) neg += 6 * t[m / 4];
        out[n] = sb(pos, neg, p);
    }
    free(t); free(ac); free(c); free(e); free(d);
}

/* out[n] = a(n) exactly, n = 1..L */
static void series_big(idx_t L, seq_t *out)
{
    seq_t *t = alloc_zero(L + 1), *ac = alloc_zero(L + 1);
    seq_t *c = alloc_zero(L + 1), *e = alloc_zero(L + 1), *d = alloc_zero(L + 1);
    idx_t k, i, n;
    for (k = 0; k <= L; k++) {
        seq_t s = 0;
        if (k == 0) {
            t[0] = 1;
            ac[0] = 1;
        } else {
            s = e[k - 1];
            if ((k - 1) % 3 == 0) s += 2 * t[(k - 1) / 3];
            t[k] = s / 3;
            s = 0;
            for (i = 0; 2 * i <= k - 1; i++) s += t[i] * ac[k - 1 - 2 * i];
            ac[k] = s;
        }
        s = 0;
        for (i = 0; i <= k; i++) s += t[i] * t[k - i];
        c[k] = s;
        s = 0;
        for (i = 0; i <= k; i++) s += c[i] * t[k - i];
        e[k] = s;
        s = 0;
        for (i = 0; i <= k; i++) s += ac[i] * ac[k - i];
        d[k] = s;
    }
    for (n = 1; n <= L; n++) {
        idx_t m = n - 1;
        seq_t q = 0, x = 0, y = 0, pos = 0, neg = 0;
        for (i = 0; i <= m; i++) q += c[i] * c[m - i];
        for (i = 0; 3 * i <= m; i++) x += t[i] * t[m - 3 * i];
        for (i = 0; 2 * i <= m; i++) y += t[i] * d[m - 2 * i];
        pos = q + 8 * x + 6 * (d[n] - 2 * ac[n]);
        if (m % 2 == 0) pos += 3 * c[m / 2];
        neg = 6 * y + 6 * (c[n] - 2 * t[n]);
        if (m % 4 == 0) neg += 6 * t[m / 4];
        out[n] = (pos - neg) / 12;
    }
    release(t, L + 1); release(ac, L + 1);
    release(c, L + 1); release(e, L + 1); release(d, L + 1);
}

seq_t a(idx_t n)
{
    static u128 *fres = NULL;
    static idx_t fcap = 0;
    static seq_t *bres = NULL;
    static idx_t bcap = 0;
    idx_t L;
    if (!seq_is_fixed_width()) {
        if (n > bcap) {
            seq_t *r;
            L = bcap + bcap / 4;
            if (L < n) L = n;
            if (L < 32) L = 32;
            r = alloc_zero(L + 1);
            series_big(L, r);
            if (bres) release(bres, bcap + 1);
            bres = r;
            bcap = L;
        }
        return bres[n];
    }
    if (n > fcap) {
        u128 *w, *v = NULL;
        u128 inv3 = inv3_128(), mask = ((u128)1 << 126) - 1;
        uint64_t inv12 = powm(12, PR - 2, PR);
        uint64_t hp = (uint64_t)(((u128)1 << 126) % PR);
        idx_t i;
        int k;
        L = fcap + fcap / 4;
        if (L < n) L = n;
        if (L < 32) L = 32;
        w = (u128 *)malloc((size_t)(L + 1) * sizeof(u128));
        series_fixed(L, 0, w);
        if (sizeof(seq_t) > 8) {
            v = (u128 *)malloc((size_t)(L + 1) * sizeof(u128));
            series_fixed(L, PR, v);
        }
        w[0] = 0;
        for (i = 1; i <= L; i++) {
            u128 r = ((w[i] >> 2) * inv3) & mask;
            if (v) {
                uint64_t ap = (uint64_t)(v[i] % PR) * inv12 % PR;
                uint64_t rp = (uint64_t)(r % PR);
                for (k = 0; k < 4; k++) {
                    if ((rp + (uint64_t)k * hp) % PR == ap) {
                        r += (u128)k << 126;
                        break;
                    }
                }
            }
            w[i] = r;
        }
        free(v);
        free(fres);
        fres = w;
        fcap = L;
    }
    return u128_to_seq(fres[n]);
}
