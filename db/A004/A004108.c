/*
 * A004108  Number of n-node unlabeled connected graphs without endpoints.
 * https://oeis.org/A004108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = inverse Euler transform of A004110, a(0) = 1, where by
 *            Burnside A004110(n) = Sum_{T partition of j, 0<=j<=n}
 *            2^E(T)/z(T) * [x^(n-j)] Prod_{b in T} (1-x^b), E(T) = Sum_{pairs
 *            of parts t,u} gcd(t,u) + Sum_{parts t} floor(t/2) (edge orbits),
 *            z(T) = Prod_k k^m_k m_k! (m_k = multiplicity of par...
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 11, u64 n <= 14, u128 n <= 19,
 *            big any n
 * Helpers:   addto, divsmall, gcd_i, mulacc, mulsmall, subfrom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/addto.h"
#include "../utils/divsmall.h"
#include "../utils/gcd_i.h"
#include "../utils/mulacc.h"
#include "../utils/mulsmall.h"
#include "../utils/subfrom.h"

static int bitlen_u(uint64_t k) { int b = 0; while (k) { b++; k >>= 1; } return b; }

static int limbs_for(idx_t n)
{
    long bits = (long)(n * (n - 1) / 2) + 160;
    idx_t k;
    for (k = 1; k <= n; k++) bits += bitlen_u(k);
    return (int)(bits / 32 + 2);
}

static int fact_limbs(idx_t n)
{
    long bits = 64;
    idx_t k;
    for (k = 1; k <= n; k++) bits += bitlen_u(k);
    return (int)(bits / 32 + 2);
}

/* ---- Burnside enumeration of graphs without endpoints ---- */
static int gN, gL, gML;
static uint32_t *gPos, *gNeg, *gMst;
static __int128 *gPoly;
static int *gCnt, *gParts, *gG;

static void addmul_shift(uint32_t *acc, const uint32_t *m, uint32_t c, long shift)
{
    long off = shift >> 5, i;
    int r = (int)(shift & 31);
    unsigned __int128 carry = 0;
    for (i = 0; i < gML && off + i < gL; i++) {
        carry += ((unsigned __int128)((uint64_t)m[i] * c) << r) + acc[off + i];
        acc[off + i] = (uint32_t)carry;
        carry >>= 32;
    }
    for (i = off + i; carry && i < gL; i++) {
        carry += acc[i];
        acc[i] = (uint32_t)carry;
        carry >>= 32;
    }
}

static void dfs(int maxp, int j, int depth, long E)
{
    __int128 coef = gPoly[gN - j];
    uint32_t *M = gMst + (size_t)depth * gML;
    int a, top, d, i;
    if (coef != 0) {
        unsigned __int128 u = coef < 0 ? (unsigned __int128)0 - (unsigned __int128)coef
                                       : (unsigned __int128)coef;
        uint32_t *acc = coef < 0 ? gNeg : gPos;
        int k;
        for (k = 0; k < 4; k++) {
            uint32_t c = (uint32_t)(u >> (32 * k));
            if (c) addmul_shift(acc, M, c, E + 32L * k);
        }
    }
    top = gN - j;
    if (maxp < top) top = maxp;
    for (a = top; a >= 1; a--) {
        long dE = a / 2;
        int lim = gN - j - a;
        uint32_t *M2 = M + gML;
        gCnt[a]++;
        for (d = 0; d < depth; d++) dE += gG[a * (gN + 1) + gParts[d]];
        gParts[depth] = a;
        memcpy(M2, M, sizeof(uint32_t) * gML);
        divsmall(M2, gML, (uint64_t)a * (uint64_t)gCnt[a]);
        for (i = lim; i >= a; i--) gPoly[i] -= gPoly[i - a];
        dfs(a, j + a, depth + 1, E + dE);
        for (i = a; i <= lim; i++) gPoly[i] += gPoly[i - a];
        gCnt[a]--;
    }
}

/* returns A004110(n) as limbs_for(n) limbs */
static uint32_t *bfun(int n)
{
    int i, k;
    uint32_t *res;
    gN = n;
    gL = limbs_for((idx_t)n);
    gML = fact_limbs((idx_t)n);
    gPos = (uint32_t *)calloc((size_t)gL, sizeof(uint32_t));
    gNeg = (uint32_t *)calloc((size_t)gL, sizeof(uint32_t));
    gMst = (uint32_t *)calloc((size_t)(n + 2) * gML, sizeof(uint32_t));
    gPoly = (__int128 *)calloc((size_t)n + 1, sizeof(__int128));
    gCnt = (int *)calloc((size_t)n + 1, sizeof(int));
    gParts = (int *)calloc((size_t)n + 1, sizeof(int));
    gG = (int *)calloc((size_t)(n + 1) * (n + 1), sizeof(int));
    for (i = 1; i <= n; i++)
        for (k = 1; k <= n; k++) gG[i * (n + 1) + k] = gcd_i(i, k);
    gMst[0] = 1;
    for (k = 2; k <= n; k++) mulsmall(gMst, gML, (uint32_t)k);
    gPoly[0] = 1;
    dfs(n, 0, 0, 0);
    subfrom(gPos, gL, gNeg, gL);
    for (k = 2; k <= n; k++) divsmall(gPos, gL, (uint64_t)k);
    res = gPos;
    free(gNeg); free(gMst); free(gPoly); free(gCnt); free(gParts); free(gG);
    return res;
}

/* ---- inverse Euler transform cache ---- */
static uint32_t **cB = NULL, **cC = NULL;
static int *cL = NULL;
static idx_t cDone = 0, cCap = 0;

static void ensure(idx_t n)
{
    idx_t m, k;
    if (n <= cDone) return;
    if (n + 1 > cCap) {
        idx_t nc = n + 1 + 16;
        cB = (uint32_t **)realloc(cB, sizeof(uint32_t *) * nc);
        cC = (uint32_t **)realloc(cC, sizeof(uint32_t *) * nc);
        cL = (int *)realloc(cL, sizeof(int) * nc);
        cCap = nc;
    }
    for (m = cDone + 1; m <= n; m++) {
        int L = limbs_for(m);
        uint32_t *b = bfun((int)m);
        uint32_t *S = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
        uint32_t *c = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
        for (k = 1; k < m; k++) mulacc(S, L, cC[k], cL[k], cB[m - k], cL[m - k]);
        memcpy(c, b, sizeof(uint32_t) * L);
        mulsmall(c, L, (uint32_t)m);
        subfrom(c, L, S, L);
        free(S);
        cB[m] = b;
        cC[m] = c;
        cL[m] = L;
        cDone = m;
    }
}

static int mobius(idx_t k)
{
    int r = 1;
    idx_t p;
    for (p = 2; p * p <= k; p++) {
        if (k % p == 0) {
            k /= p;
            if (k % p == 0) return 0;
            r = -r;
        }
    }
    if (k > 1) r = -r;
    return r;
}

seq_t a(idx_t n)
{
    int L, i, sh;
    idx_t d;
    uint32_t *pos, *neg;
    seq_t r = 0;
    if (n == 0) return (seq_t)1;
    ensure(n);
    L = cL[n];
    pos = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    neg = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    for (d = 1; d <= n; d++) {
        int mu;
        if (n % d) continue;
        mu = mobius(n / d);
        if (mu > 0) addto(pos, L, cC[d], cL[d]);
        else if (mu < 0) addto(neg, L, cC[d], cL[d]);
    }
    subfrom(pos, L, neg, L);
    divsmall(pos, L, (uint64_t)n);
    for (i = L - 1; i >= 0; i--)
        for (sh = 24; sh >= 0; sh -= 8)
            r = r * (seq_t)256 + (seq_t)((pos[i] >> sh) & 255u);
    free(pos);
    free(neg);
    return r;
}
