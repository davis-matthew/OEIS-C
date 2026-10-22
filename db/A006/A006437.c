/*
 * A006437  Number of tree-rooted toroidal maps with 3 faces and n vertices
 *          and without isthmuses.
 * https://oeis.org/A006437
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = Sum_B Sum_{m} binomial(n-1-D(m)+7, 7), where B runs over
 *            the 70 rooted genus-1 one-vertex maps with 4 edges (the cotree
 *            after contracting the spanning tree; 70 = a(1)), m = (m_1..m_7)
 *            >= 0 with m_0 = m_8 = 0 are the segment minima of the tree's
 *            contour word cut by the 8 cotree half-edges,...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static int pidx[9][9];
static uint32_t cmask[105];
static int cmult[105];
static int ncm = 0;
static uint64_t kw[78125];
static int keys_ready = 0;
static seq_t *A_tab = NULL;
static idx_t A_len = 0;

static void process_matching(const int *pt)
{
    int vis[8], i, f = 0, s, t;
    uint32_t mask = 0;
    for (i = 0; i < 8; i++) vis[i] = 0;
    for (i = 0; i < 8; i++) {
        if (!vis[i]) {
            int j = i;
            f++;
            while (!vis[j]) { vis[j] = 1; j = (pt[j] + 1) % 8; }
        }
    }
    if (f != 3) return; /* genus 1 */
    for (s = 0; s <= 6; s++)
        for (t = s + 2; t <= 8; t++) {
            int closed = 1;
            for (i = s; i < t; i++)
                if (pt[i] < s || pt[i] >= t) { closed = 0; break; }
            if (closed) mask |= 1u << pidx[s][t];
        }
    for (i = 0; i < ncm; i++)
        if (cmask[i] == mask) { cmult[i]++; return; }
    cmask[ncm] = mask;
    cmult[ncm] = 1;
    ncm++;
}

static void gen_matchings(int *pt)
{
    int i = 0, j;
    while (i < 8 && pt[i] >= 0) i++;
    if (i == 8) { process_matching(pt); return; }
    for (j = i + 1; j < 8; j++)
        if (pt[j] < 0) {
            pt[i] = j; pt[j] = i;
            gen_matchings(pt);
            pt[i] = -1; pt[j] = -1;
        }
}

static void build_keys(void)
{
    int pt[8], L[9], s, t, i, cnt = 0;
    long code;
    for (i = 0; i < 8; i++) pt[i] = -1;
    for (s = 0; s <= 6; s++)
        for (t = s + 2; t <= 8; t++) pidx[s][t] = cnt++;
    gen_matchings(pt);
    memset(kw, 0, sizeof(kw));
    L[0] = 0; L[8] = 0;
    for (code = 0; code < 2097152L; code++) {
        long x = code;
        int used = 0, r = 0, k, j;
        uint32_t pm = 0;
        uint64_t w = 0;
        int cs[7];
        long key;
        for (k = 1; k <= 7; k++) {
            L[k] = (int)(x & 7); x >>= 3;
            if (L[k]) { used |= 1 << L[k]; if (L[k] > r) r = L[k]; }
        }
        if (used != (1 << (r + 1)) - 2) continue;
        for (s = 0; s <= 6; s++)
            for (t = s + 2; t <= 8; t++) {
                int mx = L[s] > L[t] ? L[s] : L[t], mn = 99;
                for (i = s + 1; i < t; i++) if (L[i] < mn) mn = L[i];
                if (mx < mn) pm |= 1u << pidx[s][t];
            }
        for (i = 0; i < ncm; i++)
            if ((pm & cmask[i]) == 0) w += (uint64_t)cmult[i];
        if (w == 0) continue;
        for (j = 1; j <= r; j++) {
            int c = 0;
            for (k = 1; k <= 8; k++) {
                int lo = L[k - 1] < L[k] ? L[k - 1] : L[k];
                int hi = L[k - 1] < L[k] ? L[k] : L[k - 1];
                if (lo < j && j <= hi) c++;
            }
            cs[j - 1] = c / 2;
        }
        for (i = 1; i < r; i++) {
            int v = cs[i], q = i;
            while (q > 0 && cs[q - 1] < v) { cs[q] = cs[q - 1]; q--; }
            cs[q] = v;
        }
        key = 0;
        for (i = r - 1; i >= 0; i--) key = key * 5 + cs[i];
        kw[key] += w;
    }
    keys_ready = 1;
}

static void build_table(idx_t N)
{
    seq_t *F = alloc_zero(N);
    seq_t *S = alloc_zero(N);
    long key;
    idx_t i;
    int rep;
    for (key = 0; key < 78125L; key++) {
        long kk = key;
        seq_t w;
        if (kw[key] == 0) continue;
        for (i = 0; i < N; i++) S[i] = 0;
        S[0] = 1;
        while (kk) {
            idx_t c = (idx_t)(kk % 5);
            kk /= 5;
            for (i = N; i-- > 0;) {
                if (i >= c) S[i] = S[i - c];
                else S[i] = 0;
            }
            for (i = c; i < N; i++) S[i] += S[i - c];
        }
        w = (seq_t)kw[key];
        for (i = 0; i < N; i++) F[i] += w * S[i];
    }
    for (rep = 0; rep < 8; rep++)
        for (i = 1; i < N; i++) F[i] += F[i - 1];
    release(S, N);
    if (A_tab) release(A_tab, A_len);
    A_tab = F;
    A_len = N;
}

seq_t a(idx_t n)
{
    if (!keys_ready) build_keys();
    if (n > A_len) {
        idx_t N = 2 * A_len;
        if (N < n) N = n;
        if (N < 64) N = 64;
        build_table(N);
    }
    return A_tab[n - 1];
}
