/*
 * A000666  Number of symmetric relations on n nodes.
 * https://oeis.org/A000666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Sum_{partitions lambda of n} 2^c(lambda)/z(lambda), where
 *            for lambda = 1^m_1 2^m_2 ... : z(lambda) = Prod_k k^m_k*m_k! and
 *            c(lambda) = Sum_k m_k*(floor(k/2)+1) + Sum_k k*binomial(m_k,2) +
 *            Sum_{i<j} m_i*m_j*gcd(i,j) (Polya: cycles of Sym_n acting on
 *            loops and edges). Parts >= 3 are enumerat...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 13, u128 n <= 18,
 *            big any n
 * Helpers:   add_shift, bn_div_u, bn_len, bn_mul_u, g6_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/add_shift.h"
#include "../utils/bn_div_u.h"
#include "../utils/bn_len.h"
#include "../utils/bn_mul_u.h"
#include "../utils/g6_gcd.h"

static idx_t gD1, gD2;
static int TL, CL;
static uint32_t *coef;
static idx_t *pk, *pm;
static uint32_t **W;

static void record(idx_t r, idx_t A1, idx_t A2, const uint32_t *c, idx_t e)
{
    size_t id = ((size_t)r * (size_t)(gD1 + 1) + (size_t)A1) * (size_t)(gD2 + 1) + (size_t)A2;
    if (!W[id]) W[id] = (uint32_t *)calloc((size_t)TL, sizeof(uint32_t));
    add_shift(W[id], TL, c, bn_len(c, CL), e);
}

static void rec(idx_t r, idx_t maxk, idx_t depth, idx_t e, idx_t A1, idx_t A2)
{
    idx_t k, kmax, i, m;
    record(r, A1, A2, coef + (size_t)depth * CL, e);
    kmax = maxk < r ? maxk : r;
    for (k = kmax; k >= 3; k--) {
        idx_t base = 0, ee = e, rr = r, a1 = A1, a2 = A2;
        uint32_t *cur = coef + (size_t)(depth + 1) * CL;
        for (i = 0; i < depth; i++) base += pm[i] * g6_gcd(k, pk[i]);
        memcpy(cur, coef + (size_t)depth * CL, sizeof(uint32_t) * (size_t)CL);
        for (m = 1; m <= r / k; m++) {
            idx_t t;
            uint64_t prod = 1;
            for (t = 0; t < k; t++) {
                uint64_t f = (uint64_t)(rr - t);
                if (prod * f >= ((uint64_t)1 << 32)) { bn_mul_u(cur, CL, prod); prod = 1; }
                prod *= f;
            }
            if (prod > 1) bn_mul_u(cur, CL, prod);
            bn_div_u(cur, CL, (uint64_t)(k * m));
            ee += k / 2 + 1 + base + (m - 1) * k;
            rr -= k;
            a1 += 1;
            a2 += (k % 2 == 0) ? 2 : 1;
            pk[depth] = k;
            pm[depth] = m;
            rec(rr, k - 1, depth + 1, ee, a1, a2);
        }
    }
}

seq_t a(idx_t n)
{
    idx_t E = n * (n + 1) / 2, d, r, A1, A2;
    uint64_t fbits = 64;
    size_t nW, id;
    uint32_t *S, *tmp;
    seq_t res = 0;
    int i, top;

    for (d = 2; d <= n; d++) {
        idx_t x = d;
        while (x) { fbits++; x >>= 1; }
    }
    CL = (int)(fbits / 32 + 3);
    TL = (int)((E + fbits) / 32 + 4);
    gD1 = n / 3;
    gD2 = 2 * gD1;
    nW = (size_t)(n + 1) * (size_t)(gD1 + 1) * (size_t)(gD2 + 1);
    W = (uint32_t **)calloc(nW, sizeof(uint32_t *));
    coef = (uint32_t *)calloc((size_t)(n + 2) * CL, sizeof(uint32_t));
    pk = (idx_t *)calloc((size_t)(n + 2), sizeof(idx_t));
    pm = (idx_t *)calloc((size_t)(n + 2), sizeof(idx_t));
    coef[0] = 1;

    rec(n, n, 0, 0, 0, 0);

    S = (uint32_t *)calloc((size_t)TL, sizeof(uint32_t));
    tmp = (uint32_t *)calloc((size_t)TL + 3, sizeof(uint32_t));
    for (r = 0; r <= n; r++)
        for (A1 = 0; A1 <= gD1; A1++)
            for (A2 = 0; A2 <= gD2; A2++) {
                idx_t m2;
                int len;
                id = ((size_t)r * (size_t)(gD1 + 1) + (size_t)A1) * (size_t)(gD2 + 1) + (size_t)A2;
                if (!W[id]) continue;
                memset(tmp, 0, sizeof(uint32_t) * ((size_t)TL + 3));
                memcpy(tmp, W[id], sizeof(uint32_t) * (size_t)TL);
                len = bn_len(tmp, TL);
                for (m2 = 0; 2 * m2 <= r; m2++) {
                    idx_t m1 = r - 2 * m2;
                    idx_t sh = m1 + m2 + m1 * (m1 - 1) / 2 + m2 * (m2 - 1) + m1 * m2 + m1 * A1 + m2 * A2;
                    int ml;
                    add_shift(S, TL, tmp, len, sh);
                    if (2 * (m2 + 1) > r) break;
                    ml = len + 2 < TL + 3 ? len + 2 : TL + 3;
                    bn_mul_u(tmp, ml, (uint64_t)(m1 * (m1 - 1)));
                    bn_div_u(tmp, ml, (uint64_t)(m2 + 1));
                    len = bn_len(tmp, ml);
                }
                free(W[id]);
            }

    for (d = 2; d <= n; d++) bn_div_u(S, TL, (uint64_t)d);

    top = bn_len(S, TL);
    for (i = top - 1; i >= 0; i--) {
        int q;
        for (q = 0; q < 4; q++) res = (seq_t)(res << 8);
        res += (idx_t)S[i];
    }

    free(S);
    free(tmp);
    free(W);
    free(coef);
    free(pk);
    free(pm);
    return res;
}
