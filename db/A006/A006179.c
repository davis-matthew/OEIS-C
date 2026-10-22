/*
 * A006179  Witt vector *4!/4!.
 * https://oeis.org/A006179
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (w(n) - Sum_{d|n, d<n} d*a(d)^(n/d)) / n, where w(n) =
 *            (4n)!/(24*(n!)^4) = C(4n-1,n-1)*C(3n-1,n-1)*C(2n-1,n-1) is the
 *            ghost component (Witt transform of A029809), a(1) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   big_cbin, grow_zero, seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/seq_is_big.h"
#include "../utils/grow_zero.h"
#include "../utils/big_cbin.h"

/* ---------- arbitrary-precision path (seq_t is a big integer) ---------- */

static seq_t *bm = NULL;
static idx_t bcap = 0;

static seq_t spow(seq_t b, idx_t e)
{
    seq_t r = 1;
    while (e) {
        if (e & 1) r *= b;
        e >>= 1;
        if (e) b *= b;
    }
    return r;
}

static seq_t ba(idx_t n)
{
    seq_t w, s;
    idx_t d;
    if (bm[n] != 0) return bm[n];
    w = big_cbin(4 * n - 1, n - 1) * big_cbin(3 * n - 1, n - 1) * big_cbin(2 * n - 1, n - 1);
    s = 0;
    for (d = 1; d < n; d++)
        if (n % d == 0)
            s += (seq_t)d * spow(ba(d), n / d);
    bm[n] = (w - s) / (seq_t)n;
    return bm[n];
}

/* ---------- fixed-width path: exact small bignum, then truncate ---------- */

#define BL 12
#define FMAX 20
typedef struct { uint32_t v[BL]; } bn_t;

static void bn_small(bn_t *x, uint32_t s)
{
    memset(x->v, 0, sizeof(x->v));
    x->v[0] = s;
}

static void bn_mulsm(bn_t *x, uint32_t m)
{
    uint64_t c = 0, t;
    int i;
    for (i = 0; i < BL; i++) {
        t = (uint64_t)x->v[i] * m + c;
        x->v[i] = (uint32_t)t;
        c = t >> 32;
    }
}

static void bn_divsm(bn_t *x, uint32_t m)
{
    uint64_t r = 0, t;
    int i;
    for (i = BL - 1; i >= 0; i--) {
        t = (r << 32) | x->v[i];
        x->v[i] = (uint32_t)(t / m);
        r = t % m;
    }
}

static void bn_mul(bn_t *z, const bn_t *x, const bn_t *y)
{
    uint32_t out[BL];
    uint64_t c, t;
    int i, j;
    memset(out, 0, sizeof(out));
    for (i = 0; i < BL; i++) {
        c = 0;
        for (j = 0; i + j < BL; j++) {
            t = (uint64_t)x->v[i] * y->v[j] + out[i + j] + c;
            out[i + j] = (uint32_t)t;
            c = t >> 32;
        }
    }
    memcpy(z->v, out, sizeof(out));
}

static void bn_sub(bn_t *x, const bn_t *y)
{
    uint64_t b = 0, t;
    int i;
    for (i = 0; i < BL; i++) {
        t = (uint64_t)x->v[i] - y->v[i] - b;
        x->v[i] = (uint32_t)t;
        b = (t >> 32) & 1;
    }
}

static void bn_cbin(bn_t *r, uint32_t m, uint32_t k)
{
    uint32_t i;
    bn_small(r, 1);
    for (i = 1; i <= k; i++) {
        bn_mulsm(r, m - k + i);
        bn_divsm(r, i);
    }
}

static bn_t fm[FMAX + 1];
static bool fh[FMAX + 1];

static void fa(idx_t n)
{
    bn_t w, t, p;
    idx_t d, e, k;
    uint32_t nn = (uint32_t)n;
    if (fh[n]) return;
    bn_cbin(&w, 4 * nn - 1, nn - 1);
    bn_cbin(&t, 3 * nn - 1, nn - 1);
    bn_mul(&w, &w, &t);
    bn_cbin(&t, 2 * nn - 1, nn - 1);
    bn_mul(&w, &w, &t);
    for (d = 1; d < n; d++) {
        if (n % d != 0) continue;
        fa(d);
        e = n / d;
        bn_small(&p, 1);
        for (k = 0; k < e; k++)
            bn_mul(&p, &p, &fm[d]);
        bn_mulsm(&p, (uint32_t)d);
        bn_sub(&w, &p);
    }
    bn_divsm(&w, nn);
    fm[n] = w;
    fh[n] = true;
}

seq_t a(idx_t n)
{
    if (seq_is_big()) {
        if (n + 1 > bcap) {
            idx_t nc = 2 * (n + 1);
            bm = grow_zero(bm, bcap, nc);
            bcap = nc;
        }
        return ba(n);
    } else {
        seq_t r = 0;
        int i, sh;
        if (n > FMAX) return 0; /* a(n) exceeds 128 bits here */
        fa(n);
        for (i = BL - 1; i >= 0; i--)
            for (sh = 24; sh >= 0; sh -= 8) {
                r = (seq_t)(r << 8);
                r = (seq_t)(r + (seq_t)((fm[n].v[i] >> sh) & 255u));
            }
        return r;
    }
}
