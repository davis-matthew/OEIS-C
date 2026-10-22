/*
 * A007812  Number of n-node Steinhaus graphs whose complements have at least
 *          one cut-vertex.
 * https://oeis.org/A007812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   for n >= 4, a(n) = number of first rows of the Steinhaus
 *            triangle whose graph has a complement H that is connected and
 *            has a cut-vertex (a(1..3) = 0,1,3, where OEIS's small-n
 *            convention differs). Search: for each cut vertex v, DFS that
 *            splits the other vertices into C/R (no H-edge between C and R...
 * Range:     u8 n <= 16, u16 n <= 27, u32 n <= 27, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   sg_reach
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/sg_reach.h"

static int sg_n, sg_m, sg_v;
static uint64_t sg_L[64][64];
static uint64_t sg_prow[64];
static int sg_prhs[64];
static uint64_t sg_piv;
static int sg_stk[64];
static int sg_sp;
static int sg_ord[64];
static uint64_t sg_C, sg_R, sg_full, sg_vars;
static seq_t sg_cnt;

static int sg_ins(uint64_t m, int r)
{
    while (m) {
        int p = 63 - __builtin_clzll(m);
        if ((sg_piv >> p) & 1) {
            m ^= sg_prow[p];
            r ^= sg_prhs[p];
        } else {
            sg_prow[p] = m;
            sg_prhs[p] = r;
            sg_piv |= (uint64_t)1 << p;
            sg_stk[sg_sp++] = p;
            return 1;
        }
    }
    return r == 0;
}

static void sg_undo(int saved)
{
    while (sg_sp > saved) {
        int p = sg_stk[--sg_sp];
        sg_piv &= ~((uint64_t)1 << p);
    }
}

static void sg_check(uint64_t x)
{
    uint64_t g[64], h[64], row;
    int i, w, n = sg_n, u0 = sg_ord[0];
    for (i = 0; i < n; i++) g[i] = 0;
    row = (x << 1) & sg_full;
    for (i = 0; i + 1 < n; i++) {
        uint64_t bits;
        if (i > 0) row = ((row << 1) ^ row) & sg_full & ~((((uint64_t)1) << (i + 1)) - 1);
        bits = row;
        while (bits) {
            int j = __builtin_ctzll(bits);
            bits &= bits - 1;
            g[i] |= (uint64_t)1 << j;
            g[j] |= (uint64_t)1 << i;
        }
    }
    for (i = 0; i < n; i++) h[i] = sg_full & ~g[i] & ~((uint64_t)1 << i);
    if (sg_reach(h, sg_full, u0) != sg_full) return;
    if (sg_reach(h, sg_full & ~((uint64_t)1 << sg_v), u0) != sg_C) return;
    for (w = 0; w < sg_v; w++) {
        uint64_t allowed = sg_full & ~((uint64_t)1 << w);
        if (sg_reach(h, allowed, __builtin_ctzll(allowed)) != allowed) return;
    }
    sg_cnt++;
}

static void sg_leaf(void)
{
    uint64_t F = sg_vars & ~sg_piv, s = 0;
    do {
        uint64_t x = s, P = sg_piv;
        while (P) {
            int p = __builtin_ctzll(P);
            P &= P - 1;
            if ((__builtin_popcountll(sg_prow[p] & x) & 1) ^ sg_prhs[p])
                x |= (uint64_t)1 << p;
        }
        sg_check(x);
        s = (s - F) & F;
    } while (s);
}

static uint64_t sg_form(int u, int r)
{
    return u < r ? sg_L[u][r] : sg_L[r][u];
}

static void sg_dfs(int t)
{
    int u, saved, ok;
    uint64_t bu, bits;
    if (t == sg_m) {
        if (sg_R) sg_leaf();
        return;
    }
    u = sg_ord[t];
    bu = (uint64_t)1 << u;
    saved = sg_sp;

    ok = 1;
    bits = sg_R;
    while (bits) {
        int r = __builtin_ctzll(bits);
        bits &= bits - 1;
        if (!sg_ins(sg_form(u, r), 1)) { ok = 0; break; }
    }
    if (ok) {
        sg_C |= bu;
        sg_dfs(t + 1);
        sg_C &= ~bu;
    }
    sg_undo(saved);

    if (t > 0) {
        ok = 1;
        bits = sg_C;
        while (bits) {
            int c = __builtin_ctzll(bits);
            bits &= bits - 1;
            if (!sg_ins(sg_form(u, c), 1)) { ok = 0; break; }
        }
        if (ok) {
            sg_R |= bu;
            sg_dfs(t + 1);
            sg_R &= ~bu;
        }
        sg_undo(saved);
    }
}

static seq_t sg_count(int n)
{
    int i, j, v;
    sg_n = n;
    sg_full = (n >= 64) ? ~(uint64_t)0 : (((uint64_t)1 << n) - 1);
    sg_vars = (((uint64_t)1 << (n - 1)) - 1);
    for (j = 1; j < n; j++) sg_L[0][j] = (uint64_t)1 << (j - 1);
    for (i = 1; i < n; i++)
        for (j = i + 1; j < n; j++)
            sg_L[i][j] = sg_L[i - 1][j - 1] ^ sg_L[i - 1][j];
    sg_cnt = 0;
    for (v = 0; v < n; v++) {
        int k = 0;
        sg_v = v;
        for (i = 0; i < n; i++)
            if (i != v) sg_ord[k++] = i;
        sg_m = k;
        sg_piv = 0;
        sg_sp = 0;
        sg_C = 0;
        sg_R = 0;
        sg_dfs(0);
    }
    return sg_cnt;
}

seq_t a(idx_t n)
{
    static seq_t memo[65];
    static bool have[65];
    int m;
    if (n < 1 || n > 64) return 0;
    if (n == 1) return 0;
    if (n == 2) return 1;
    if (n == 3) return 3;
    m = (int)n;
    if (!have[m]) {
        memo[m] = sg_count(m);
        have[m] = true;
    }
    return memo[m];
}
