/*
 * A005801  Generalized tangent numbers of type 3^(2n+1).
 * https://oeis.org/A005801
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n/3^(2n+1) * Sum_{i=0..2n+1}
 *            (-1)^(i+1)*binomial(2n+1,i)*A002105(n+i+1), where A002105(k) =
 *            A000182(k)/2^(k-1) is the reduced tangent number; this is the
 *            OEIS formula a(n) = 1/3^(2n+1) * Sum_{i=0..2n+1}
 *            (-1)^(i+1)*2^-i*binomial(2n+1,i)*A000182(n+i+1). A002105 comes
 *            from the derivative po...
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 8 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static seq_t *grow_zero(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    if (p == NULL) return NULL;
    mem_set(p, 0, (size_t)newlen * sizeof(seq_t));
    if (old != NULL) {
        mem_copy(p, old, (size_t)oldlen * sizeof(seq_t));
        mem_free(old);
    }
    return p;
}

static void release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

static bool seq_is_fixed_width(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 256; i++) {
        t *= 2;
        if (t == 0)
            return true;
    }
    return false;
}

/* exact division by 3 modulo 2^w: x * (-0x5555...) */
static seq_t div3_fixed(seq_t x)
{
    idx_t w = (idx_t)sizeof(seq_t) * 8;
    seq_t s = 0;
    idx_t j;
    for (j = 0; 2 * j < w; j++) s += (seq_t)(x << (2 * j));
    return (seq_t)((seq_t)0 - s);
}

static idx_t hmul(idx_t k)
{
    return (k % 2 == 0) ? k / 2 : k;
}

static seq_t *coef = NULL;   /* derivative-polynomial coefficients */
static idx_t coef_len = 0;
static idx_t steps = 0;      /* number of derivatives taken so far */
static seq_t *red = NULL;    /* red[k] = A002105(k), k >= 1 */
static idx_t red_len = 0;

static void ensure_reduced(idx_t K)
{
    idx_t need_steps = 2 * K - 1;
    if (coef == NULL) {
        coef_len = 4;
        coef = alloc_zero(coef_len);
        coef[1] = 1;
        steps = 0;
    }
    if (red_len < K + 1) {
        idx_t nl = K + 1;
        if (red == NULL) red = alloc_zero(nl);
        else red = grow_zero(red, red_len, nl);
        red_len = nl;
    }
    if (coef_len < need_steps + 3) {
        idx_t nl = need_steps + 3;
        coef = grow_zero(coef, coef_len, nl);
        coef_len = nl;
    }
    while (steps < need_steps) {
        idx_t s = steps + 1;
        idx_t p = (s + 1) & 1;
        idx_t j;
        for (j = p; j <= s + 1; j += 2) {
            seq_t v = coef[j + 1] * hmul(j + 1);
            if (j >= 1) v += coef[j - 1] * hmul(j - 1);
            coef[j] = v;
        }
        steps = s;
        if (s % 2 == 1) red[(s + 1) / 2] = coef[0];
    }
}

seq_t a(idx_t n)
{
    idx_t m = 2 * n + 1;
    idx_t K = 3 * n + 2;
    idx_t i, lvl;
    seq_t *d;
    seq_t S;
    bool fixed = seq_is_fixed_width();

    ensure_reduced(K);

    d = alloc_zero(m + 1);
    for (i = 0; i <= m; i++) d[i] = red[n + 1 + i];
    /* m-th forward difference: Sum (-1)^(m-i) binomial(m,i) R(n+1+i), m odd */
    for (lvl = 1; lvl <= m; lvl++)
        for (i = 0; i + lvl <= m; i++)
            d[i] = (seq_t)(d[i + 1] - d[i]);
    S = d[0];
    release(d, m + 1);

    for (i = 0; i < m; i++) {
        if (fixed) S = div3_fixed(S);
        else S /= 3;
    }
    for (i = 0; i < n; i++) S += S;
    return S;
}
