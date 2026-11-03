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
 * Validated: standard, pass, 25 terms through n = 98 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
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

static uint64_t mulm(uint64_t a, uint64_t b, uint64_t m)
{
    return (uint64_t)((unsigned __int128)a * b % m);
}

static uint64_t powm(uint64_t b, uint64_t e, uint64_t m)
{
    uint64_t r = 1 % m;
    b %= m;
    while (e) {
        if (e & 1) r = mulm(r, b, m);
        b = mulm(b, b, m);
        e >>= 1;
    }
    return r;
}

static uint64_t subm(uint64_t a, uint64_t b, uint64_t m)
{
    return (a + m - b) % m;
}

static void charpoly_mod(int s, const long long *A, uint64_t p, uint64_t *out)
{
    uint64_t *H = (uint64_t *)malloc(sizeof(uint64_t) * (size_t)s * s);
    for (int i = 0; i < s * s; i++) H[i] = (uint64_t)A[i] % p;
    for (int j = 0; j < s - 2; j++) {
        int piv = -1;
        for (int i = j + 1; i < s; i++)
            if (H[i * s + j]) { piv = i; break; }
        if (piv < 0) continue;
        if (piv != j + 1) {
            for (int k = 0; k < s; k++) {
                uint64_t tmp = H[piv * s + k];
                H[piv * s + k] = H[(j + 1) * s + k];
                H[(j + 1) * s + k] = tmp;
            }
            for (int k = 0; k < s; k++) {
                uint64_t tmp = H[k * s + piv];
                H[k * s + piv] = H[k * s + j + 1];
                H[k * s + j + 1] = tmp;
            }
        }
        uint64_t inv = powm(H[(j + 1) * s + j], p - 2, p);
        for (int i = j + 2; i < s; i++) {
            uint64_t u = mulm(H[i * s + j], inv, p);
            if (!u) continue;
            for (int k = 0; k < s; k++)
                H[i * s + k] = subm(H[i * s + k], mulm(u, H[(j + 1) * s + k], p), p);
            for (int k = 0; k < s; k++)
                H[k * s + j + 1] = (H[k * s + j + 1] + mulm(u, H[k * s + i], p)) % p;
        }
    }
    int w = s + 1;
    uint64_t *P = (uint64_t *)calloc((size_t)w * w, sizeof(uint64_t));
    P[0] = 1;
    for (int k = 1; k <= s; k++) {
        uint64_t *Pk = P + (size_t)k * w;
        const uint64_t *Pq = P + (size_t)(k - 1) * w;
        uint64_t h = H[(k - 1) * s + (k - 1)];
        for (int d = 0; d <= k; d++) {
            uint64_t v = d > 0 ? Pq[d - 1] : 0;
            if (d < k) v = subm(v, mulm(h, Pq[d], p), p);
            Pk[d] = v;
        }
        uint64_t t = 1;
        for (int i = 1; i < k; i++) {
            t = mulm(t, H[(k - i) * s + (k - i - 1)], p);
            uint64_t c = mulm(t, H[(k - i - 1) * s + (k - 1)], p);
            if (!c) continue;
            const uint64_t *Pr = P + (size_t)(k - i - 1) * w;
            for (int d = 0; d <= k - i - 1; d++)
                Pk[d] = subm(Pk[d], mulm(c, Pr[d], p), p);
        }
    }
    memcpy(out, P + (size_t)s * w, sizeof(uint64_t) * (size_t)w);
    free(P);
    free(H);
}

static int fnd(int *p, int x)
{
    while (p[x] != x) x = p[x];
    return x;
}

static bool mrp(uint64_t n)
{
    static const uint64_t B[12] = {2, 3, 5, 7, 11, 13, 17, 19, 23, 29, 31, 37};
    if (n < 2) return false;
    for (int i = 0; i < 12; i++)
        if (n % B[i] == 0) return n == B[i];
    uint64_t d = n - 1;
    int r = 0;
    while (!(d & 1)) { d >>= 1; r++; }
    for (int i = 0; i < 12; i++) {
        uint64_t x = powm(B[i], d, n);
        if (x == 1 || x == n - 1) continue;
        bool comp = true;
        for (int k = 1; k < r; k++) {
            x = mulm(x, x, n);
            if (x == n - 1) { comp = false; break; }
        }
        if (comp) return false;
    }
    return true;
}

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
