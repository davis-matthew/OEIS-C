/*
 * A003313  Length of shortest addition chain for n.
 * https://oeis.org/A003313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, look
 * Formula:   no closed form; a(n) is found by an iterative-deepening
 *            depth-first search over ascending addition chains 1 = c0 < c1 <
 *            ... < cL = n, with pruning c_k * 2^(L-k) >= n, starting from the
 *            proven lower bound floor(log_2 n) + [v(n) >= 2] + [v(n) >= 3],
 *            where v(n) = A000120(n)
 * Range:     u8 n <= 9550, u16 n <= 9550, u32 n <= 9550, u64 n <= 9550,
 *            u128 n <= 9550, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

#define A3313_MAXD 72

static uint64_t a3313_ch[A3313_MAXD + 2];
static uint64_t a3313_tried[A3313_MAXD + 1][2700];
static int a3313_L;
static uint64_t a3313_N;

static int a3313_dfs(int k)
{
    uint64_t last = a3313_ch[k];
    int rem, i, j, nt, t, dup;
    if (last == a3313_N) return 1;
    if (k >= a3313_L) return 0;
    rem = a3313_L - k;
    if (rem < 63 && (last << rem) < a3313_N) return 0;
    nt = 0;
    for (i = k; i >= 0; i--) {
        if (2 * a3313_ch[i] <= last) break;
        for (j = i; j >= 0; j--) {
            uint64_t s = a3313_ch[i] + a3313_ch[j];
            if (s <= last) break;
            if (s > a3313_N) continue;
            if (rem - 1 < 63 && (s << (rem - 1)) < a3313_N) break;
            dup = 0;
            for (t = 0; t < nt; t++)
                if (a3313_tried[k][t] == s) { dup = 1; break; }
            if (dup) continue;
            if (nt < 2700) a3313_tried[k][nt++] = s;
            a3313_ch[k + 1] = s;
            if (a3313_dfs(k + 1)) return 1;
        }
    }
    return 0;
}

seq_t a(idx_t n)
{
    uint64_t x;
    int lam = 0, v = 0, L;
    if (n <= 1) return 0;
    x = (uint64_t)n;
    while (x > 1) { x >>= 1; lam++; }
    x = (uint64_t)n;
    while (x) { v += (int)(x & 1); x >>= 1; }
    L = lam + (v >= 2 ? 1 : 0) + (v >= 3 ? 1 : 0);
    a3313_N = (uint64_t)n;
    a3313_ch[0] = 1;
    for (;; L++) {
        if (L > A3313_MAXD) break;
        a3313_L = L;
        if (a3313_dfs(0)) return (seq_t)L;
    }
    return (seq_t)L;
}
