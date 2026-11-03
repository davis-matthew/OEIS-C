/*
 * A006420  Number of rooted planar maps with 3 vertices and n faces and no
 *          isthmuses.
 * https://oeis.org/A006420
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [t^(n-2)] (1+2t-4t^2) / ((1-t)^(n+3) * (1-2t)^4) (by
 *            Lagrange inversion of G.f. x^2*(1+2g-4g^2)/((1-g)^4*(1-2g)^5), g
 *            = x*C(x), x = g(1-g)), i.e. with U(j) = binomial(n+2+j, j) and V
 *            = U convolved 4 times with 2^k: a(n) = V(n-2) + 2*V(n-3) -
 *            4*V(n-4)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 29, u128 n <= 60,
 *            big any n
 * Validated: standard, pass, 39 terms through n = 430 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
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

static int seq_is_big(void)
{
    seq_t x = 1;
    int i;
    for (i = 0; i < 130; i++) x = x * 2;
    return !(x == 0);
}

static seq_t u128_to_seq(unsigned __int128 u)
{
    seq_t r = 0;
    int k;
    for (k = 3; k >= 0; k--) {
        r = r * 65536ULL;
        r = r * 65536ULL;
        r += (seq_t)(uint64_t)(uint32_t)(u >> (32 * k));
    }
    return r;
}

typedef unsigned __int128 a006420_u128;

static a006420_u128 a006420_inv(a006420_u128 d) {
    a006420_u128 x = d;
    for (int i = 0; i < 7; i++) x *= (a006420_u128)2 - d * x;
    return x;
}

static seq_t a006420_fixed(idx_t n) {
    idx_t m = n - 2;
    a006420_u128 *v = (a006420_u128 *)malloc((size_t)(m + 1) * sizeof(a006420_u128));
    a006420_u128 odd = 1;
    unsigned long long e = 0;
    v[0] = 1;
    for (idx_t j = 1; j <= m; j++) {
        unsigned long long num = (unsigned long long)(n + 2 + j);
        unsigned long long den = (unsigned long long)j;
        while ((num & 1ULL) == 0) { num >>= 1; e++; }
        while ((den & 1ULL) == 0) { den >>= 1; e--; }
        odd = odd * (a006420_u128)num * a006420_inv((a006420_u128)den);
        v[j] = (e >= 128) ? (a006420_u128)0 : (odd << e);
    }
    for (int pass = 0; pass < 4; pass++)
        for (idx_t j = 1; j <= m; j++) v[j] += 2 * v[j - 1];
    a006420_u128 r = v[m];
    if (m >= 1) r += 2 * v[m - 1];
    if (m >= 2) r -= 4 * v[m - 2];
    free(v);
    return u128_to_seq(r);
}

static seq_t a006420_big(idx_t n) {
    idx_t m = n - 2;
    seq_t *v = alloc_zero(m + 1);
    v[0] = 1;
    for (idx_t j = 1; j <= m; j++)
        v[j] = v[j - 1] * (seq_t)(n + 2 + j) / (seq_t)j;
    for (int pass = 0; pass < 4; pass++)
        for (idx_t j = 1; j <= m; j++) v[j] += 2 * v[j - 1];
    seq_t r = v[m];
    if (m >= 1) r += 2 * v[m - 1];
    if (m >= 2) r -= 4 * v[m - 2];
    release(v, m + 1);
    return r;
}

seq_t a(idx_t n) {
    if (n < 2) return 0;
    if (seq_is_big()) return a006420_big(n);
    return a006420_fixed(n);
}
