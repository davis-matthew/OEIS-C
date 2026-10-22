/*
 * A006082  Number of labeled projective plane trees (or "flat" trees) with n
 *          nodes.
 * https://oeis.org/A006082
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside over the dihedral group acting on noncrossing perfect
 *            matchings of 2k points, k = n-1: a(n) = (2*A002995(n) +
 *            A126120(n-2) + A001405(n-1)) / 4, with A002995(n) = (1/(2k)) *
 *            (C(k) + binomial(k,floor(k/2)) + Sum_{e|k, e>=3}
 *            phi(e)*binomial(2k/e,k/e)), C(k) = Catalan number; A126120(m) =
 *            C(...
 * Range:     u8 n <= 10, u16 n <= 15, u32 n <= 24, u64 n <= 41, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static void bn_mul(uint32_t *x, size_t *len, uint64_t m)
{
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < *len; i++) {
        uint64_t t = (uint64_t)x[i] * m + c;
        x[i] = (uint32_t)t;
        c = t >> 32;
    }
    while (c) {
        x[(*len)++] = (uint32_t)c;
        c >>= 32;
    }
}

static uint64_t bn_div(uint32_t *x, size_t *len, uint64_t dv)
{
    uint64_t r = 0;
    size_t i = *len;
    while (i > 0) {
        uint64_t cur;
        i--;
        cur = (r << 32) | x[i];
        x[i] = (uint32_t)(cur / dv);
        r = cur % dv;
    }
    while (*len > 0 && x[*len - 1] == 0) (*len)--;
    return r;
}

static void bn_add(uint32_t *x, size_t *xl, const uint32_t *y, size_t yl)
{
    size_t n = *xl > yl ? *xl : yl;
    size_t i;
    uint64_t c = 0;
    for (i = 0; i < n; i++) {
        uint64_t t = (uint64_t)x[i] + (i < yl ? y[i] : 0) + c;
        x[i] = (uint32_t)t;
        c = t >> 32;
    }
    if (c) x[n++] = (uint32_t)c;
    *xl = n;
}

static void bn_binom(uint32_t *x, size_t *len, size_t cap, uint64_t N, uint64_t K)
{
    uint64_t i;
    memset(x, 0, cap * sizeof(uint32_t));
    x[0] = 1;
    *len = 1;
    for (i = 1; i <= K; i++) {
        bn_mul(x, len, N - K + i);
        bn_div(x, len, i);
    }
}

static uint64_t tot(uint64_t m)
{
    uint64_t r = m, p;
    for (p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            while (m % p == 0) m /= p;
            r -= r / p;
        }
    }
    if (m > 1) r -= r / m;
    return r;
}

seq_t a(idx_t n)
{
    uint64_t k, e;
    size_t cap, al = 0, tl = 0, i;
    uint32_t *acc, *tmp;
    seq_t r = 0;

    if (n <= 1) return (seq_t)1;
    k = (uint64_t)n - 1;
    cap = (size_t)(2 * k / 32 + 8);
    acc = (uint32_t *)calloc(cap, sizeof(uint32_t));
    tmp = (uint32_t *)calloc(cap, sizeof(uint32_t));

    /* e = 1: Catalan C(k) */
    bn_binom(tmp, &tl, cap, 2 * k, k);
    bn_div(tmp, &tl, k + 1);
    bn_add(acc, &al, tmp, tl);

    /* e = 2: binomial(k, floor(k/2)); also reflection term k*A001405(k) */
    bn_binom(tmp, &tl, cap, k, k / 2);
    bn_add(acc, &al, tmp, tl);
    bn_mul(tmp, &tl, k);
    bn_add(acc, &al, tmp, tl);

    /* reflection term k*A126120(k-1) */
    if (k % 2 == 1) {
        uint64_t j = (k - 1) / 2;
        bn_binom(tmp, &tl, cap, 2 * j, j);
        bn_div(tmp, &tl, j + 1);
        bn_mul(tmp, &tl, k);
        bn_add(acc, &al, tmp, tl);
    }

    /* e >= 3 dividing k */
    for (e = 3; e <= k; e++) {
        if (k % e != 0) continue;
        bn_binom(tmp, &tl, cap, 2 * k / e, k / e);
        bn_mul(tmp, &tl, tot(e));
        bn_add(acc, &al, tmp, tl);
    }

    bn_div(acc, &al, 4 * k);

    i = al;
    while (i > 0) {
        int s;
        i--;
        for (s = 24; s >= 0; s -= 8) {
            r = (seq_t)(r * 256u);
            r += (seq_t)((acc[i] >> s) & 255u);
        }
    }

    free(acc);
    free(tmp);
    return r;
}
