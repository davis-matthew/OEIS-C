/*
 * A006178  Witt vector *3!/3!.
 * https://oeis.org/A006178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A029808 Witt transform) = (g(n) - Sum_{d|n, d<n}
 *            d*a(d)^(n/d)) / n, where g(n) = Sum_{d|n} d*a(d)^(n/d) =
 *            (3n)!/(6*(n!)^3) = binomial(3n,n)*binomial(2n,n)/6
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   big_g, big_pow, grow_zero, seq_is_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/seq_is_big.h"
#include "../utils/u128_to_seq.h"
#include "../utils/grow_zero.h"
#include "../utils/big_g.h"
#include "../utils/big_pow.h"

typedef unsigned __int128 u128;

/* ---------- arbitrary-precision path ---------- */
static seq_t *bm = NULL;
static bool *bh = NULL;
static idx_t bcap = 0;

static seq_t big_a(idx_t n)
{
    if (bh[n]) return bm[n];
    seq_t s = big_g(n);
    idx_t d;
    for (d = 1; d < n; d++)
        if (n % d == 0) s -= (seq_t)d * big_pow(big_a(d), n / d);
    s /= (seq_t)n;
    bm[n] = s;
    bh[n] = true;
    return s;
}

/* ---------- fixed-width path: a(n) exact modulo 2^(128 - v2(n)) ---------- */
static u128 *fm = NULL;
static bool *fh = NULL;
static idx_t fcap = 0;

static u128 f_pow(u128 b, idx_t e)
{
    u128 r = 1;
    while (e) {
        if (e & 1) r *= b;
        e >>= 1;
        if (e) b *= b;
    }
    return r;
}

static u128 f_inv(u128 m)
{
    u128 x = m;
    int k;
    for (k = 0; k < 7; k++) x *= (u128)2 - m * x;
    return x;
}

static u128 f_odd(u128 x, long *e)
{
    while ((x & 1) == 0) { x >>= 1; (*e)++; }
    return x;
}

/* odd part of binomial(N,k) mod 2^128; adds its 2-adic valuation to *e */
static u128 f_binom(idx_t N, idx_t k, long *e)
{
    u128 o = 1;
    idx_t i;
    for (i = 0; i < k; i++) {
        long ed = 0;
        o *= f_odd((u128)(N - i), e);
        o *= f_inv(f_odd((u128)(i + 1), &ed));
        *e -= ed;
    }
    return o;
}

static u128 f_g(idx_t n)
{
    long e = 0;
    u128 o = f_binom(3 * n, n, &e);
    o *= f_binom(2 * n, n, &e);
    o *= f_inv((u128)3);
    e -= 1;
    if (e >= 128) return 0;
    return o << e;
}

static u128 f_a(idx_t n)
{
    if (fh[n]) return fm[n];
    u128 s = f_g(n);
    idx_t d;
    for (d = 1; d < n; d++)
        if (n % d == 0) s -= (u128)d * f_pow(f_a(d), n / d);
    idx_t m = n;
    int t = 0;
    while ((m & 1) == 0) { m >>= 1; t++; }
    s = (s >> t) * f_inv((u128)m);
    fm[n] = s;
    fh[n] = true;
    return s;
}

seq_t a(idx_t n)
{
    if (seq_is_big()) {
        if (n + 1 > bcap) {
            idx_t nc = (n + 1) * 2;
            bm = grow_zero(bm, bcap, nc);
            bh = (bool *)realloc(bh, sizeof(bool) * nc);
            memset(bh + bcap, 0, sizeof(bool) * (nc - bcap));
            bcap = nc;
        }
        return big_a(n);
    } else {
        if (n + 1 > fcap) {
            idx_t nc = (n + 1) * 2;
            fm = (u128 *)realloc(fm, sizeof(u128) * nc);
            fh = (bool *)realloc(fh, sizeof(bool) * nc);
            memset(fh + fcap, 0, sizeof(bool) * (nc - fcap));
            fcap = nc;
        }
        return u128_to_seq(f_a(n));
    }
}
