/*
 * A007511  a(n) is the smallest number greater than a(n-1) that is
 *          expressible as the sum of two squares in more ways than a(n-1).
 * https://oeis.org/A007511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search, not a closed form. A025426(m) = (B - [m square] + [m =
 *            2*square])/2, where B = prod (e_i+1) over primes p_i == 1 (mod
 *            4) dividing m. a(1) = 2, and a(n) is the smallest m with
 *            A025426(m) > A025426(a(n-1)). The code finds it with a DFS over
 *            m = 2^c * prod p_i^e_i, c in {0,1}, where the expo...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 19, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"

#define NPR 80
static uint64_t pr[NPR];
static int pr_ready = 0;

static unsigned long long g_v;
static seq_t g_best;
static unsigned long long g_bestf;
static seq_t g_lim;
static bool g_fixed;

static void init_primes(void) {
    int c = 0;
    uint64_t x;
    for (x = 5; c < NPR; x += 4) {
        uint64_t d;
        bool ok = true;
        for (d = 2; d * d <= x; d++)
            if (x % d == 0) { ok = false; break; }
        if (ok) pr[c++] = x;
    }
    pr_ready = 1;
}

static bool fits(uint64_t p) {
    if (!g_fixed) return true;
    if (sizeof(seq_t) >= 8) return true;
    return p <= (uint64_t)g_lim;
}

static void dfs(int i, int maxe, seq_t P, unsigned long long B, bool alleven) {
    if (!alleven) {
        if (B / 2 > g_v) {
            if (P < g_best) { g_best = P; g_bestf = B / 2; }
            return;
        }
    } else {
        if ((B - 1) / 2 > g_v) {
            if (P < g_best) { g_best = P; g_bestf = (B - 1) / 2; }
            return;
        }
        if ((B + 1) / 2 > g_v) {
            if (P <= g_best / 2 && P + P < g_best) { g_best = P + P; g_bestf = (B + 1) / 2; }
            return;
        }
    }
    if (i >= NPR) return;
    if (!fits(pr[i])) return;
    seq_t p = (seq_t)pr[i];
    seq_t Q = P;
    int e;
    for (e = 1; e <= maxe; e++) {
        if (Q > g_best / p) break;
        Q *= p;
        dfs(i + 1, e, Q, B * (unsigned long long)(e + 1), alleven && (e % 2 == 0));
    }
}

static seq_t *memo = NULL;
static unsigned long long *memof = NULL;
static idx_t memo_cnt = 0, memo_cap = 0;

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (!pr_ready) init_primes();
    g_fixed = seq_is_fixed_width();
    if (g_fixed) { g_lim = 0; g_lim--; }
    if (memo == NULL) {
        memo_cap = 64;
        memo = (seq_t *)malloc(memo_cap * sizeof(seq_t));
        memof = (unsigned long long *)malloc(memo_cap * sizeof(unsigned long long));
        memo[0] = 2;
        memof[0] = 1;
        memo_cnt = 1;
    }
    while (memo_cnt < n) {
        if (memo_cnt >= memo_cap) {
            idx_t nc = memo_cap * 2;
            seq_t *nm = (seq_t *)malloc(nc * sizeof(seq_t));
            unsigned long long *nf = (unsigned long long *)malloc(nc * sizeof(unsigned long long));
            idx_t k;
            for (k = 0; k < memo_cnt; k++) { nm[k] = memo[k]; nf[k] = memof[k]; }
            free(memo); free(memof);
            memo = nm; memof = nf; memo_cap = nc;
        }
        g_v = memof[memo_cnt - 1];
        /* initial bound: squarefree product of the first primes */
        {
            seq_t P0 = 1;
            unsigned long long B0 = 1;
            bool capped = false;
            int i;
            for (i = 0; i < NPR; i++) {
                if (B0 / 2 > g_v) break;
                if (!fits(pr[i])) { capped = true; break; }
                seq_t p = (seq_t)pr[i];
                if (g_fixed && P0 > g_lim / p) { capped = true; break; }
                P0 *= p;
                B0 *= 2;
            }
            if (capped || B0 / 2 <= g_v) {
                g_best = g_fixed ? g_lim : P0;
                g_bestf = g_v + 1;
            } else {
                g_best = P0;
                g_bestf = B0 / 2;
            }
        }
        dfs(0, 1000000, (seq_t)1, 1ULL, true);
        memo[memo_cnt] = g_best;
        memof[memo_cnt] = g_bestf;
        memo_cnt++;
    }
    return memo[n - 1];
}
