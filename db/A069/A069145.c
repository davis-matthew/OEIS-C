/*
 * A069145  Numbers k such that A069088(k) divides k.
 * https://oeis.org/A069145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n-th k with A069088(k) | k, where A069088 is
 *            multiplicative with A069088(p^e) = (p+1)(e+1)/2 for odd e and
 *            (p+1)e/2 + 1 for even e. Search: depth-first enumeration of
 *            factorizations k = prod p^e (primes in decreasing order) up to a
 *            bound X, tracking which prime powers of A069088(k) are not...
 * Range:     u8 n <= 7, u16 n <= 62, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 59 terms through n = 999 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

typedef struct { uint64_t p; int vm; int vF; } A069145_ent;
#define A069145_MAXE 96

static uint64_t *A069145_P = 0;
static size_t A069145_NP = 0;
static uint32_t *A069145_SPF = 0;
static uint64_t A069145_SPFL = 0;
static uint64_t A069145_X = 0;

static uint64_t *A069145_T = 0;
static size_t A069145_TC = 0, A069145_TCap = 0;
static uint64_t A069145_done = 0;

static void A069145_record(uint64_t m)
{
    if (A069145_TC == A069145_TCap) {
        size_t nc = A069145_TCap ? A069145_TCap * 2 : 1024;
        A069145_T = (uint64_t *)realloc(A069145_T, nc * sizeof(uint64_t));
        A069145_TCap = nc;
    }
    A069145_T[A069145_TC++] = m;
}

static int A069145_addf(A069145_ent *E, int cnt, uint64_t s, int k)
{
    int j;
    for (j = 0; j < cnt; j++)
        if (E[j].p == s) { E[j].vF += k; return cnt; }
    E[cnt].p = s; E[cnt].vm = 0; E[cnt].vF = k;
    return cnt + 1;
}

static int A069145_factor_add(A069145_ent *E, int cnt, uint64_t g)
{
    if (g > A069145_SPFL) {
        size_t i;
        for (i = 0; i < A069145_NP; i++) {
            uint64_t p = A069145_P[i];
            if (p * p > g) break;
            if (g % p == 0) {
                int k = 0;
                while (g % p == 0) { g /= p; k++; }
                cnt = A069145_addf(E, cnt, p, k);
                if (g <= A069145_SPFL) break;
            }
        }
        if (g > A069145_SPFL) { if (g > 1) cnt = A069145_addf(E, cnt, g, 1); return cnt; }
    }
    while (g > 1) {
        uint64_t p = A069145_SPF[g];
        int k = 0;
        while (g % p == 0) { g /= p; k++; }
        cnt = A069145_addf(E, cnt, p, k);
    }
    return cnt;
}

static void A069145_dfs(const A069145_ent *E, int cnt, uint64_t m, size_t qidx);

static void A069145_child(const A069145_ent *E, int cnt, uint64_t m,
                          uint64_t r, int e, uint64_t pe, size_t idx)
{
    A069145_ent NE[A069145_MAXE];
    int nc = cnt, j, found = 0;
    uint64_t g;
    for (j = 0; j < cnt; j++) NE[j] = E[j];
    for (j = 0; j < nc; j++)
        if (NE[j].p == r) { NE[j].vm = e; found = 1; break; }
    if (!found) { NE[nc].p = r; NE[nc].vm = e; NE[nc].vF = 0; nc++; }
    if (e & 1) g = (r + 1) * (uint64_t)((e + 1) / 2);
    else g = (r + 1) * (uint64_t)(e / 2) + 1;
    nc = A069145_factor_add(NE, nc, g);
    for (j = 0; j < nc; j++)
        if (NE[j].p >= r && NE[j].vF > NE[j].vm) return;
    A069145_dfs(NE, nc, m * pe, idx);
}

static void A069145_dfs(const A069145_ent *E, int cnt, uint64_t m, size_t qidx)
{
    uint64_t lim = A069145_X / m, need = 1, L = 0, lim2;
    int D = 0, k, t;
    size_t i = 0;
    for (k = 0; k < cnt; k++) {
        int diff = E[k].vF - E[k].vm;
        if (diff > 0) {
            uint64_t p = E[k].p;
            for (t = 0; t < diff; t++) {
                if (need > lim / p) return;
                need *= p;
            }
            if (p > L) { L = p; D = diff; }
        }
    }
    if (need == 1) A069145_record(m);
    lim2 = lim / need;
    if (L) {
        size_t lo = 0, hi = qidx;
        uint64_t nr = need, cap, pe = 1;
        int e;
        while (lo < hi) {
            size_t mid = (lo + hi) / 2;
            if (A069145_P[mid] < L) lo = mid + 1; else hi = mid;
        }
        if (lo >= qidx || A069145_P[lo] != L) return;
        for (t = 0; t < D; t++) { nr /= L; pe *= L; }
        cap = lim / nr;
        e = D;
        for (;;) {
            A069145_child(E, cnt, m, L, e, pe, lo);
            if (pe > cap / L) break;
            pe *= L; e++;
        }
        i = lo + 1;
    }
    for (; i < qidx; i++) {
        uint64_t r = A069145_P[i], pe = r;
        int e = 1;
        if (r > lim2) break;
        for (;;) {
            A069145_child(E, cnt, m, r, e, pe, i);
            if (pe > lim2 / r) break;
            pe *= r; e++;
        }
    }
}

static int A069145_cmp(const void *x, const void *y)
{
    uint64_t u = *(const uint64_t *)x, v = *(const uint64_t *)y;
    return (u > v) - (u < v);
}

static void A069145_build(uint64_t X)
{
    uint64_t S = 1, i, j;
    A069145_ent root[1];
    while ((S + 1) <= X / (S + 1)) S++;
    free(A069145_SPF);
    free(A069145_P);
    A069145_SPFL = S + 64;
    A069145_SPF = (uint32_t *)calloc((size_t)A069145_SPFL + 1, sizeof(uint32_t));
    A069145_NP = 0;
    for (i = 2; i <= A069145_SPFL; i++) {
        if (A069145_SPF[i] == 0) {
            if (i <= S) A069145_NP++;
            for (j = i; j <= A069145_SPFL; j += i)
                if (A069145_SPF[j] == 0) A069145_SPF[j] = (uint32_t)i;
        }
    }
    A069145_P = (uint64_t *)malloc((A069145_NP + 1) * sizeof(uint64_t));
    A069145_NP = 0;
    for (i = 2; i <= S; i++)
        if (A069145_SPF[i] == i) A069145_P[A069145_NP++] = i;
    A069145_X = X;
    A069145_TC = 0;
    A069145_dfs(root, 0, 1, A069145_NP);
    qsort(A069145_T, A069145_TC, sizeof(uint64_t), A069145_cmp);
    A069145_done = X;
}

seq_t a(idx_t n)
{
    uint64_t X;
    if (n < 1) return (seq_t)0;
    X = A069145_done ? A069145_done : (1ULL << 20);
    if (!A069145_done) A069145_build(X);
    while ((uint64_t)A069145_TC < (uint64_t)n) {
        if (X >= (1ULL << 58)) X = (1ULL << 62);
        else X *= 16;
        A069145_build(X);
        if (X >= (1ULL << 62)) break;
    }
    if ((uint64_t)A069145_TC < (uint64_t)n) return (seq_t)0;
    return (seq_t)A069145_T[n - 1];
}
