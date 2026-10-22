/*
 * A007124  Number of strength 1 Eulerian graphs with n nodes, 2 of odd
 *          degree.
 * https://oeis.org/A007124
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{partitions p of n} (c2(p) + binomial(c1(p),2)) *
 *            2^(m(p) - e(p) - max(o(p)-1,0)) / z(p), where c1, c2 are the
 *            numbers of parts equal to 1 and 2, e and o are the numbers of
 *            even and odd parts, m(p) = Sum_i floor(p_i/2) + Sum_{i<j}
 *            gcd(p_i,p_j) is the number of edge orbits, and z(p) = P...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 11, u64 n <= 15, u128 n <= 20,
 *            big any n
 * Helpers:   gcd_i
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd_i.h"

static int CL_, AL_;
static uint32_t *coef_, *acc_, *tmp_;
static int *plen_, *pcnt_;

static void bn_div(uint32_t *v, int len, uint32_t d)
{
    uint64_t r = 0;
    int i;
    for (i = len - 1; i >= 0; i--) {
        uint64_t cur = (r << 32) | v[i];
        v[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
}

static void bn_mul(uint32_t *v, int len, uint32_t s)
{
    uint64_t carry = 0;
    int i;
    for (i = 0; i < len; i++) {
        uint64_t cur = (uint64_t)v[i] * s + carry;
        v[i] = (uint32_t)cur;
        carry = cur >> 32;
    }
}

/* acc += (c * s) << k */
static void bn_addmulshift(const uint32_t *c, uint32_t s, long long k)
{
    int tlen = CL_ + 1, i;
    long long ws = k / 32;
    int b = (int)(k % 32);
    uint64_t carry = 0;
    for (i = 0; i < CL_; i++) {
        uint64_t cur = (uint64_t)c[i] * s + carry;
        tmp_[i] = (uint32_t)cur;
        carry = cur >> 32;
    }
    tmp_[CL_] = (uint32_t)carry;
    tmp_[CL_ + 1] = 0;
    carry = 0;
    for (i = 0; i <= tlen; i++) {
        uint32_t v;
        uint64_t cur;
        if (b == 0) v = tmp_[i];
        else v = (uint32_t)(((uint64_t)tmp_[i] << b) | (i ? (tmp_[i - 1] >> (32 - b)) : 0));
        cur = (uint64_t)acc_[ws + i] + v + carry;
        acc_[ws + i] = (uint32_t)cur;
        carry = cur >> 32;
    }
    for (i = (int)(ws + tlen + 1); carry && i < AL_; i++) {
        uint64_t cur = (uint64_t)acc_[i] + carry;
        acc_[i] = (uint32_t)cur;
        carry = cur >> 32;
    }
}

static void rec(int maxL, int rem, int depth, long long m, int e, int o, int c1, int c2)
{
    uint32_t *cf = coef_ + (size_t)depth * CL_;
    int top, L;
    if (rem == 0) {
        uint32_t S = (uint32_t)(c2 + c1 * (c1 - 1) / 2);
        if (S) {
            long long k = m - e - (o > 0 ? o - 1 : 0);
            bn_addmulshift(cf, S, k);
        }
        return;
    }
    top = maxL < rem ? maxL : rem;
    for (L = top; L >= 1; L--) {
        long long g = 0;
        int i, c;
        uint32_t *nx = cf + CL_;
        if (L == 1 && c1 == 0 && c2 == 0 && rem < 2) continue;
        for (i = 0; i < depth; i++) g += (long long)pcnt_[i] * gcd_i(L, plen_[i]);
        memcpy(nx, cf, (size_t)CL_ * sizeof(uint32_t));
        for (c = 1; c * L <= rem; c++) {
            long long nm;
            bn_div(nx, CL_, (uint32_t)L);
            bn_div(nx, CL_, (uint32_t)c);
            if (L == 1 && c != rem) continue;
            plen_[depth] = L;
            pcnt_[depth] = c;
            nm = m + (long long)c * (L / 2) + (long long)c * g + (long long)c * (c - 1) / 2 * L;
            rec(L - 1, rem - c * L, depth + 1, nm,
                e + (L % 2 == 0 ? c : 0), o + (L % 2 ? c : 0),
                L == 1 ? c : c1, L == 2 ? c : c2);
        }
    }
}

seq_t a(idx_t n)
{
    int nn = (int)n, lg = 1, i, d;
    long long cbits;
    seq_t r = 0;
    if (n < 2) return 0;
    while ((1 << lg) <= nn) lg++;
    CL_ = (nn * lg + 64) / 32 + 1;
    cbits = (long long)nn * (nn - 1) / 2;
    AL_ = (int)(cbits / 32) + CL_ + 6;
    coef_ = (uint32_t *)calloc((size_t)(nn + 2) * CL_, sizeof(uint32_t));
    acc_ = (uint32_t *)calloc((size_t)AL_, sizeof(uint32_t));
    tmp_ = (uint32_t *)calloc((size_t)CL_ + 2, sizeof(uint32_t));
    plen_ = (int *)calloc((size_t)nn + 1, sizeof(int));
    pcnt_ = (int *)calloc((size_t)nn + 1, sizeof(int));

    coef_[0] = 1;
    for (d = 2; d <= nn; d++) bn_mul(coef_, CL_, (uint32_t)d);

    rec(nn, nn, 0, 0, 0, 0, 0, 0);

    for (d = 2; d <= nn; d++) bn_div(acc_, AL_, (uint32_t)d);

    i = AL_ - 1;
    while (i > 0 && acc_[i] == 0) i--;
    for (; i >= 0; i--) {
        r = (seq_t)(r << 16);
        r += (seq_t)(acc_[i] >> 16);
        r = (seq_t)(r << 16);
        r += (seq_t)(acc_[i] & 0xFFFFu);
    }

    free(coef_); free(acc_); free(tmp_); free(plen_); free(pcnt_);
    return r;
}
