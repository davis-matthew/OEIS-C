/*
 * A000250  Number of symmetric reflexive relations on n nodes: (1/2)*A000666.
 * https://oeis.org/A000250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = (1/2)*A000666(n), where by Burnside / cycle index
 *            A000666(n) = Sum_{partitions 1^m1 2^m2 ... of n} 2^f / Prod_k
 *            (k^mk * mk!), f = Sum_k mk*(floor(k/2)+1) + Sum_k k*C(mk,2) +
 *            Sum_{i<j} mi*mj*gcd(i,j). Computed exactly with a private
 *            multi-limb integer, then converted to seq_t
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 9, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   a250_add, a250_divsmall, a250_gcd, a250_mulsmall
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a250_add.h"
#include "../utils/a250_divsmall.h"
#include "../utils/a250_gcd.h"
#include "../utils/a250_mulsmall.h"

static int a250_N, a250_L;
static uint32_t *a250_buck;   /* (fmax+1) buckets of L limbs */
static uint32_t *a250_cbuf;   /* per-depth coefficient n!/z_partial */
static int a250_pk[256], a250_pm[256];

static void a250_dfs(int depth, int kmax, int rem, long f)
{
    int L = a250_L, k;
    uint32_t *cur = a250_cbuf + (size_t)depth * L;
    uint32_t *nxt = cur + L;
    if (rem == 0) {
        a250_add(a250_buck + (size_t)f * L, cur, L);
        return;
    }
    if (kmax > rem) kmax = rem;
    for (k = kmax; k >= 1; k--) {
        long g = 0, ff = f;
        int i, m;
        for (i = 0; i < depth; i++)
            g += (long)a250_pm[i] * a250_gcd(k, a250_pk[i]);
        memcpy(nxt, cur, sizeof(uint32_t) * (size_t)L);
        for (m = 1; m * k <= rem; m++) {
            a250_divsmall(nxt, L, (uint32_t)(k * m));
            ff += (k / 2 + 1) + (long)(m - 1) * k + g;
            a250_pk[depth] = k;
            a250_pm[depth] = m;
            a250_dfs(depth + 1, k - 1, rem - m * k, ff);
        }
    }
}

seq_t a(idx_t n)
{
    int N = (int)n, L, D, k, i;
    long fmax, f;
    double bits = 0;
    int LT;
    uint32_t *T;
    seq_t r = 0;

    a250_N = N;
    for (k = 2; k <= N; k++) {
        int b = 0; int t = k;
        while (t) { b++; t >>= 1; }
        bits += b;
    }
    L = (int)(bits / 32) + 2;
    a250_L = L;
    fmax = (long)N * (N + 1) / 2;
    D = 2;
    while ((long)D * (D + 1) / 2 <= N) D++;
    D += 2;

    a250_buck = (uint32_t *)calloc((size_t)(fmax + 1) * L, sizeof(uint32_t));
    a250_cbuf = (uint32_t *)calloc((size_t)(D + 2) * L, sizeof(uint32_t));
    a250_cbuf[0] = 1;
    for (k = 2; k <= N; k++) a250_mulsmall(a250_cbuf, L, (uint32_t)k);

    a250_dfs(0, N, N, 0);

    LT = (int)((fmax + 32L * L) / 32) + 3;
    T = (uint32_t *)calloc((size_t)LT, sizeof(uint32_t));
    for (f = fmax; f >= 0; f--) {
        uint32_t carry = 0;
        for (i = 0; i < LT; i++) {
            uint32_t nc = T[i] >> 31;
            T[i] = (T[i] << 1) | carry;
            carry = nc;
        }
        a250_add(T, a250_buck + (size_t)f * L, L);
        {
            /* propagate carry beyond L limbs */
            int j = L;
            uint64_t c2 = 0;
            /* a250_add only handles L limbs; redo with full carry */
            (void)c2; (void)j;
        }
    }
    /* Note: carry beyond L handled below by recomputation-free approach */
    free(T);

    /* Recompute Horner with full-width carry propagation */
    T = (uint32_t *)calloc((size_t)LT, sizeof(uint32_t));
    for (f = fmax; f >= 0; f--) {
        uint32_t carry = 0;
        uint64_t c = 0;
        const uint32_t *B = a250_buck + (size_t)f * L;
        for (i = 0; i < LT; i++) {
            uint32_t nc = T[i] >> 31;
            T[i] = (T[i] << 1) | carry;
            carry = nc;
        }
        for (i = 0; i < LT; i++) {
            uint64_t s = (uint64_t)T[i] + (i < L ? B[i] : 0) + c;
            T[i] = (uint32_t)s;
            c = s >> 32;
            if (i >= L && c == 0) break;
        }
    }
    for (k = 2; k <= N; k++) a250_divsmall(T, LT, (uint32_t)k);
    a250_divsmall(T, LT, 2);

    for (i = LT - 1; i >= 0; i--) {
        int s;
        for (s = 24; s >= 0; s -= 8) {
            r = (seq_t)(r * 256u);
            r += (unsigned)((T[i] >> s) & 0xFFu);
        }
    }
    free(T);
    free(a250_buck);
    free(a250_cbuf);
    return r;
}
