/*
 * A006610  Number of n-node trees not determined by their spectra.
 * https://oeis.org/A006610
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   search, not a closed formula. Enumerate all free trees on n
 *            nodes, rooted at a centroid (or at a centroid edge when there
 *            are two). For each tree, compute its matching polynomial, which
 *            for trees is the same as the characteristic polynomial. a(n) =
 *            number of trees whose polynomial is shared with...
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 19, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   next_level
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/next_level.h"

#define MAXN 48
#define MAXK (MAXN / 2 + 2)

static uint64_t gU[MAXN][MAXK], gM[MAXN][MAXK];
static int gsz[MAXN];

static void merge_child(int p, int c)
{
    int sp = gsz[p], sc = gsz[c];
    int dp = sp / 2, dc = sc / 2, dn = (sp + sc) / 2;
    uint64_t nu[MAXK], nm[MAXK], fc[MAXK];
    int i, j;
    for (i = 0; i <= dn; i++) { nu[i] = 0; nm[i] = 0; }
    for (j = 0; j <= dc; j++) fc[j] = gU[c][j] + gM[c][j];
    for (i = 0; i <= dp; i++) {
        uint64_t u = gU[p][i], m = gM[p][i];
        if (!u && !m) continue;
        for (j = 0; j <= dc; j++) {
            nu[i + j] += u * fc[j];
            nm[i + j] += m * fc[j];
            if (i + j + 1 <= dn) nm[i + j + 1] += u * gU[c][j];
        }
    }
    for (i = 0; i <= dn; i++) { gU[p][i] = nu[i]; gM[p][i] = nm[i]; }
    gsz[p] = sp + sc;
}

/* F = all matchings of the rooted tree, U = matchings with the root unmatched */
static void tree_poly(const int *L, int len, uint64_t *F, uint64_t *U)
{
    int par[MAXN], last[MAXN];
    int i, k, K = len / 2;
    for (i = 0; i < len; i++) {
        gsz[i] = 1;
        for (k = 0; k <= K + 1; k++) { gU[i][k] = 0; gM[i][k] = 0; }
        gU[i][0] = 1;
        par[i] = i ? last[L[i] - 1] : -1;
        last[L[i]] = i;
    }
    for (i = len - 1; i >= 1; i--) merge_child(par[i], i);
    for (k = 0; k <= K; k++) { F[k] = gU[0][k] + gM[0][k]; U[k] = gU[0][k]; }
}

static uint32_t *rows;
static size_t nrows, capelems;
static int gW;

static void push_row(const uint64_t *P, int W)
{
    int k;
    size_t need = (nrows + 1) * (size_t)W;
    if (need > capelems) {
        size_t nc = capelems ? capelems : 4096;
        while (nc < need) nc *= 2;
        rows = (uint32_t *)realloc(rows, nc * sizeof(uint32_t));
        capelems = nc;
    }
    for (k = 1; k <= W; k++) rows[nrows * (size_t)W + k - 1] = (uint32_t)P[k];
    nrows++;
}

static int cmp_rows(const void *a, const void *b)
{
    const uint32_t *x = (const uint32_t *)a, *y = (const uint32_t *)b;
    int k;
    for (k = 0; k < gW; k++) {
        if (x[k] < y[k]) return -1;
        if (x[k] > y[k]) return 1;
    }
    return 0;
}

static uint64_t compute(int n)
{
    int L[MAXN];
    uint64_t F[MAXK], U[MAXK];
    int K = n / 2, W = K, i, k;
    uint64_t result = 0;
    size_t r, s;

    nrows = 0;
    /* unique centroid at the root */
    for (i = 0; i < n; i++) L[i] = i;
    do {
        int start = 1;
        bool ok = true;
        for (i = 2; i <= n; i++) {
            if (i == n || L[i] == 1) {
                if (2 * (i - start) >= n) { ok = false; break; }
                start = i;
            }
        }
        if (ok) {
            tree_poly(L, n, F, U);
            push_row(F, W);
        }
    } while (next_level(L, n));

    /* two centroids: pairs of rooted trees of size n/2 */
    if (n % 2 == 0) {
        int m = n / 2, km = m / 2, cnt = 0, cap = 64;
        uint64_t *tf = (uint64_t *)malloc((size_t)cap * (km + 1) * sizeof(uint64_t));
        uint64_t *tu = (uint64_t *)malloc((size_t)cap * (km + 1) * sizeof(uint64_t));
        int a, b, x, y;
        for (i = 0; i < m; i++) L[i] = i;
        do {
            if (cnt == cap) {
                cap *= 2;
                tf = (uint64_t *)realloc(tf, (size_t)cap * (km + 1) * sizeof(uint64_t));
                tu = (uint64_t *)realloc(tu, (size_t)cap * (km + 1) * sizeof(uint64_t));
            }
            tree_poly(L, m, F, U);
            for (k = 0; k <= km; k++) {
                tf[(size_t)cnt * (km + 1) + k] = F[k];
                tu[(size_t)cnt * (km + 1) + k] = U[k];
            }
            cnt++;
        } while (next_level(L, m));
        for (a = 0; a < cnt; a++) {
            const uint64_t *fa = tf + (size_t)a * (km + 1), *ua = tu + (size_t)a * (km + 1);
            for (b = a; b < cnt; b++) {
                const uint64_t *fb = tf + (size_t)b * (km + 1), *ub = tu + (size_t)b * (km + 1);
                for (k = 0; k <= K; k++) F[k] = 0;
                for (x = 0; x <= km; x++)
                    for (y = 0; y <= km; y++) {
                        if (x + y <= K) F[x + y] += fa[x] * fb[y];
                        if (x + y + 1 <= K) F[x + y + 1] += ua[x] * ub[y];
                    }
                push_row(F, W);
            }
        }
        free(tf);
        free(tu);
    }

    gW = W;
    qsort(rows, nrows, (size_t)W * sizeof(uint32_t), cmp_rows);
    r = 0;
    while (r < nrows) {
        s = r + 1;
        while (s < nrows && cmp_rows(rows + r * (size_t)W, rows + s * (size_t)W) == 0) s++;
        if (s - r >= 2) result += (uint64_t)(s - r);
        r = s;
    }
    return result;
}

seq_t a(idx_t n)
{
    static uint64_t memo[MAXN];
    static bool done[MAXN];
    if (n < 8 || n >= MAXN) return (seq_t)0;
    if (!done[n]) {
        memo[n] = compute((int)n);
        done[n] = true;
    }
    return (seq_t)memo[n];
}
