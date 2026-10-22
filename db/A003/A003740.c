/*
 * A003740  Number of spanning trees with degrees 1 and 3 in W_5 X P_2n.
 * https://oeis.org/A003740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   transfer matrix. b(m) counts spanning trees of W_5 X P_m with
 *            all degrees 1 or 3. The state between two layers is the set of
 *            vertical edges plus their connectivity, and states are merged by
 *            the D4 symmetry of W_5. a(n) = b(2n) = F.T.(T^2)^(n-1).e0. The
 *            first s terms are computed by iterating T. A...
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 12,
 *            big any n
 * Helpers:   alloc_zero, charpoly_mod, fnd, grow_zero, mrp, mulm, powm,
 *            release, subm
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/alloc_zero.h"
#include "../utils/grow_zero.h"
#include "../utils/release.h"
#include "../utils/charpoly_mod.h"
#include "../utils/fnd.h"
#include "../utils/mrp.h"
#include "../utils/mulm.h"
#include "../utils/powm.h"
#include "../utils/subm.h"

#define NV 5
#define MAXS 256
static const int EU[8] = {0, 0, 0, 0, 1, 2, 3, 4};
static const int EV[8] = {1, 2, 3, 4, 2, 3, 4, 1};

static int st_key[MAXS];
static int key_idx[7776];
static int ns = 0;
static long long Tm[MAXS][MAXS];
static long long Fv[MAXS];

static void decode(int key, int *lab)
{
    for (int v = 0; v < NV; v++) { lab[v] = key % 6; key /= 6; }
}

static int encode_canon(const int *lab)
{
    int map[8];
    for (int i = 0; i < 8; i++) map[i] = 0;
    int nxt = 1, key = 0, mul = 1;
    for (int v = 0; v < NV; v++) {
        int l = lab[v], c = 0;
        if (l) { if (!map[l]) map[l] = nxt++; c = map[l]; }
        key += c * mul;
        mul *= 6;
    }
    return key;
}

static int canon(const int *lab)
{
    int best = -1;
    for (int r = 0; r < 4; r++)
        for (int f = 0; f < 2; f++) {
            int nl[NV];
            nl[0] = lab[0];
            for (int i = 0; i < 4; i++) {
                int j = f ? (r - i + 4) % 4 : (r + i) % 4;
                nl[1 + j] = lab[1 + i];
            }
            int k = encode_canon(nl);
            if (best < 0 || k < best) best = k;
        }
    return best;
}

static int idx_of(int key)
{
    if (key_idx[key] < 0) { key_idx[key] = ns; st_key[ns] = key; ns++; }
    return key_idx[key];
}

static void build(void)
{
    for (int i = 0; i < 7776; i++) key_idx[i] = -1;
    int lab0[NV] = {0, 0, 0, 0, 0};
    idx_of(canon(lab0));
    for (int q = 0; q < ns; q++) {
        int lab[NV];
        decode(st_key[q], lab);
        for (int E = 0; E < 256; E++) {
            int par[NV], deg[NV];
            bool ok = true;
            for (int v = 0; v < NV; v++) { par[v] = v; deg[v] = lab[v] ? 1 : 0; }
            for (int v = 0; v < NV; v++)
                for (int w = 0; w < v; w++)
                    if (lab[v] && lab[v] == lab[w]) {
                        int rv = fnd(par, v), rw = fnd(par, w);
                        if (rv != rw) par[rv] = rw;
                    }
            for (int e = 0; e < 8 && ok; e++) {
                if (!((E >> e) & 1)) continue;
                int x = EU[e], y = EV[e];
                int rx = fnd(par, x), ry = fnd(par, y);
                if (rx == ry) { ok = false; break; }
                if (++deg[x] > 3 || ++deg[y] > 3) { ok = false; break; }
                par[rx] = ry;
            }
            if (!ok) continue;
            int up[NV], nup = 0, nroots = 0;
            int hasup[NV] = {0, 0, 0, 0, 0};
            for (int v = 0; v < NV; v++) {
                up[v] = (deg[v] % 2 == 0);
                nup += up[v];
                int r = fnd(par, v);
                if (r == v) nroots++;
                if (up[v]) hasup[r] = 1;
            }
            if (nup == 0) {
                if (nroots == 1) Fv[q]++;
                continue;
            }
            bool good = true;
            for (int v = 0; v < NV; v++)
                if (fnd(par, v) == v && !hasup[v]) good = false;
            if (!good) continue;
            int nl[NV];
            for (int v = 0; v < NV; v++) nl[v] = up[v] ? fnd(par, v) + 1 : 0;
            int t = idx_of(canon(nl));
            Tm[t][q]++;
        }
    }
}

static seq_t *memo = NULL;
static idx_t have = 0, mcap = 0;
static int S = 0;
static seq_t *cpos = NULL, *cneg = NULL;
static bool *ppos = NULL, *pneg = NULL;

static void setup(void)
{
    build();
    int s = ns;
    long long *A = (long long *)malloc(sizeof(long long) * (size_t)s * s);
    for (int i = 0; i < s; i++)
        for (int j = 0; j < s; j++) {
            long long acc = 0;
            for (int k = 0; k < s; k++) acc += Tm[i][k] * Tm[k][j];
            A[i * s + j] = acc;
        }
    unsigned long long R = 0;
    for (int j = 0; j < s; j++) {
        unsigned long long cs = 0;
        for (int i = 0; i < s; i++) cs += (unsigned long long)A[i * s + j];
        if (cs > R) R = cs;
    }
    int lg = 0;
    while (lg < 63 && (1ULL << lg) <= R) lg++;
    int bits = s * (lg + 1) + 2;
    int np = bits / 61 + 1;
    uint64_t *pr = (uint64_t *)malloc(sizeof(uint64_t) * np);
    uint64_t cand = (1ULL << 62) - 1;
    for (int i = 0; i < np; ) {
        if (mrp(cand)) pr[i++] = cand;
        cand -= 2;
    }
    uint64_t *res = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)np * (s + 1));
    for (int i = 0; i < np; i++) charpoly_mod(s, A, pr[i], res + (size_t)i * (s + 1));
    uint64_t *inv = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)np * np);
    for (int j = 0; j < np; j++)
        for (int i = 0; i < np; i++)
            inv[j * np + i] = (i == j) ? 0 : powm(pr[j] % pr[i], pr[i] - 2, pr[i]);

    cpos = alloc_zero((idx_t)s);
    cneg = alloc_zero((idx_t)s);
    ppos = (bool *)calloc((size_t)s, sizeof(bool));
    pneg = (bool *)calloc((size_t)s, sizeof(bool));
    seq_t M = 1;
    for (int i = 0; i < np; i++) M = M * (seq_t)pr[i];
    uint64_t *t = (uint64_t *)malloc(sizeof(uint64_t) * np);
    for (int k = 0; k < s; k++) {
        bool allz = true;
        for (int i = 0; i < np; i++) {
            uint64_t p = pr[i];
            uint64_t v = res[(size_t)i * (s + 1) + k];
            for (int j = 0; j < i; j++)
                v = mulm(subm(v, t[j] % p, p), inv[j * np + i], p);
            t[i] = v;
            if (v) allz = false;
        }
        if (allz) continue;
        seq_t x = (seq_t)t[np - 1];
        for (int i = np - 2; i >= 0; i--) x = x * (seq_t)pr[i] + (seq_t)t[i];
        if (x * 2 > M) { cpos[k] = M - x; ppos[k] = true; }
        else { cneg[k] = x; pneg[k] = true; }
    }
    free(t); free(inv); free(res); free(pr); free(A);

    mcap = (idx_t)s + 2;
    memo = alloc_zero(mcap);
    seq_t *y = alloc_zero((idx_t)s);
    seq_t *ny = alloc_zero((idx_t)s);
    y[0] = 1;
    for (int L = 1; L <= 2 * s - 1; L++) {
        for (int i = 0; i < s; i++) {
            seq_t acc = 0;
            for (int j = 0; j < s; j++)
                if (Tm[i][j]) acc += y[j] * (seq_t)(uint64_t)Tm[i][j];
            ny[i] = acc;
        }
        seq_t *sw = y; y = ny; ny = sw;
        if (L & 1) {
            seq_t v = 0;
            for (int i = 0; i < s; i++)
                if (Fv[i]) v += y[i] * (seq_t)(uint64_t)Fv[i];
            memo[(L + 1) / 2] = v;
        }
    }
    release(y, (idx_t)s);
    release(ny, (idx_t)s);
    have = (idx_t)s;
    S = s;
}

seq_t a(idx_t n)
{
    if (!S) setup();
    if (n > have) {
        if (n >= mcap) {
            idx_t nc = mcap;
            while (nc <= n) nc *= 2;
            memo = grow_zero(memo, mcap, nc);
            mcap = nc;
        }
        for (idx_t m = have + 1; m <= n; m++) {
            seq_t ps = 0, ng = 0;
            for (int k = 0; k < S; k++) {
                idx_t j = m - (idx_t)S + (idx_t)k;
                if (ppos[k]) ps += cpos[k] * memo[j];
                if (pneg[k]) ng += cneg[k] * memo[j];
            }
            memo[m] = ps - ng;
        }
        have = n;
    }
    return memo[n];
}
