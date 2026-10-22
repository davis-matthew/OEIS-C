/*
 * A004137  Maximal number of edges in a graceful graph on n nodes.
 * https://oeis.org/A004137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, hard, more
 * Formula:   for n <= 13, an exhaustive search for complete rulers with n
 *            marks (the longest length L for which marks can measure every
 *            distance 1..L); for n >= 14, the Wichmann ruler length: a(n) =
 *            max over r,s >= 0 with 4r+s+3 = n of 4r(r+s+2) + 3(s+1)
 * Range:     u8 n <= 26, u16 n <= 26, u32 n <= 26, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   pc64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/pc64.h"

static int g_L;
static uint64_t g_full;

static uint64_t add_mark(uint64_t S, uint64_t cov, int x)
{
    int i;
    for (i = 0; i <= g_L; i++) {
        if ((S >> i) & 1ULL) {
            int d = x > i ? x - i : i - x;
            cov |= 1ULL << d;
        }
    }
    return cov;
}

static bool dfs(uint64_t S, uint64_t cov, int m, int k)
{
    uint64_t unc = g_full & ~cov;
    int D, p;
    if (unc == 0) return true;
    if (k <= 0) return false;
    if (pc64(unc) > k * m + k * (k - 1) / 2) return false;
    D = g_L;
    while (!((unc >> D) & 1ULL)) D--;
    /* one new mark */
    for (p = 0; p + D <= g_L; p++) {
        int q = p + D;
        bool ip = (S >> p) & 1ULL, iq = (S >> q) & 1ULL;
        if (ip && !iq) {
            if (dfs(S | (1ULL << q), add_mark(S, cov, q), m + 1, k - 1)) return true;
        } else if (!ip && iq) {
            if (dfs(S | (1ULL << p), add_mark(S, cov, p), m + 1, k - 1)) return true;
        }
    }
    /* two new marks */
    if (k >= 2) {
        for (p = 0; p + D <= g_L; p++) {
            int q = p + D;
            if (!((S >> p) & 1ULL) && !((S >> q) & 1ULL)) {
                uint64_t S1 = S | (1ULL << p);
                uint64_t c1 = add_mark(S, cov, p);
                c1 = add_mark(S1, c1, q);
                if (dfs(S1 | (1ULL << q), c1, m + 2, k - 2)) return true;
            }
        }
    }
    return false;
}

static bool feasible(int n, int L)
{
    uint64_t S, cov;
    g_L = L;
    g_full = (((uint64_t)1 << (L + 1)) - 1) & ~(uint64_t)1;
    /* by symmetry, distance L-1 is covered by a mark at 1 */
    S = (1ULL << 0) | (1ULL << 1) | (1ULL << L);
    cov = (1ULL << 1) | (1ULL << L) | (1ULL << (L - 1));
    return dfs(S, cov, 3, n - 3);
}

static int small_cache[14];

static int small_max(int n)
{
    int L;
    if (n == 1) return 0;
    if (n == 2) return 1;
    if (small_cache[n]) return small_cache[n];
    L = n - 1;
    while (feasible(n, L + 1)) L++;
    small_cache[n] = L;
    return L;
}

seq_t a(idx_t n)
{
    seq_t best = 0;
    idx_t r;
    if (n <= 13) return (seq_t)small_max((int)n);
    for (r = 0; 4 * r + 3 <= n; r++) {
        idx_t s = n - 3 - 4 * r;
        seq_t v = (seq_t)4 * (seq_t)r * ((seq_t)r + (seq_t)s + 2) + (seq_t)3 * ((seq_t)s + 1);
        if (v > best) best = v;
    }
    return best;
}
