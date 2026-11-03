/*
 * A005953  Theta series of P_{11a} packing.
 * https://oeis.org/A005953
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: th3(q^2)^11 + 34*th3(q^2)^7*th2(q^2)^4 +
 *            20*th3(q^2)^5*th2(q^2)^6 + 17*th3(q^2)^3*th2(q^2)^8, where
 *            th3(q^2) = 1 + 2*Sum_{k>=1} q^(2k^2) and th2(q^2) =
 *            2*q^(1/2)*Sum_{m>=0} q^(2m(m+1)). This is Construction A applied
 *            to the (11,72,4) code with weight distribution 1, 34 (w=4), 20
 *            (w=6), 17 (...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 20, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 21 terms through n = 20 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static void release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

/* s *= Sum_{m>=0} q^(2m(m+1)), truncated to len terms, in place */
static void a005953_mulP(seq_t *s, idx_t len)
{
    idx_t i, m;
    for (i = len; i-- > 0;) {
        seq_t acc = s[i];
        for (m = 1; 2 * m * (m + 1) <= i; m++)
            acc += s[i - 2 * m * (m + 1)];
        s[i] = acc;
    }
}

/* s *= th3(q^2) = 1 + 2*Sum q^(2k^2), truncated to len terms, in place */
static void a005953_mulT(seq_t *s, idx_t len)
{
    idx_t i, k;
    for (i = len; i-- > 0;) {
        seq_t acc = s[i];
        for (k = 1; 2 * k * k <= i; k++)
            acc += 2 * s[i - 2 * k * k];
        s[i] = acc;
    }
}

static seq_t *a005953_cache = NULL;
static idx_t a005953_len = 0;

static void a005953_term(seq_t *res, idx_t len, idx_t shift, seq_t coef,
                         int tpow, int ppow)
{
    seq_t *tmp;
    idx_t i;
    int j;
    if (shift >= len) return;
    tmp = alloc_zero(len);
    tmp[shift] = coef;
    for (j = 0; j < tpow; j++) a005953_mulT(tmp, len);
    for (j = 0; j < ppow; j++) a005953_mulP(tmp, len);
    for (i = 0; i < len; i++) res[i] += tmp[i];
    release(tmp, len);
}

seq_t a(idx_t n)
{
    if (n >= a005953_len) {
        idx_t len = n + 1;
        seq_t *res;
        if (len < 2 * a005953_len) len = 2 * a005953_len;
        if (len < 64) len = 64;
        res = alloc_zero(len);
        /* th3^11 */
        a005953_term(res, len, 0, (seq_t)1, 11, 0);
        /* 34 * th3^7 * th2^4 : th2^4 = 16 q^2 P^4 */
        a005953_term(res, len, 2, (seq_t)544, 7, 4);
        /* 20 * th3^5 * th2^6 : th2^6 = 64 q^3 P^6 */
        a005953_term(res, len, 3, (seq_t)1280, 5, 6);
        /* 17 * th3^3 * th2^8 : th2^8 = 256 q^4 P^8 */
        a005953_term(res, len, 4, (seq_t)4352, 3, 8);
        if (a005953_cache) release(a005953_cache, a005953_len);
        a005953_cache = res;
        a005953_len = len;
    }
    return a005953_cache[n];
}
