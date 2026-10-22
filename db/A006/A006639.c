/*
 * A006639  Restricted postage stamp problem.
 * https://oeis.org/A006639
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   exhaustive search. a(n) = 3*m, where m is the largest value for
 *            which some set A of n positive denominations with max(A) = m has
 *            every amount 0..3m expressible as a sum of at most 3 elements of
 *            A. The search is a depth-first search over sorted sets, pruned
 *            by the smallest amount not yet covered
 * Range:     u8 n <= 8, u16 n <= 8, u32 n <= 8, u64 n <= 8, u128 n <= 8,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static int K;
static long long L;
static long long best;
static uint8_t *buf;

static int covers(const uint8_t *c, long long m, long long g) {
    long long v;
    for (v = g; v <= 3 * m; v++) {
        if (v < L && c[v] <= 3) continue;
        if (v >= m && c[v - m] <= 2) continue;
        if (v >= 2 * m && c[v - 2 * m] <= 1) continue;
        if (v == 3 * m) continue;
        return 0;
    }
    return 1;
}

static void dfs(int i, long long last) {
    uint8_t *c = buf + (size_t)i * (size_t)L;
    long long g = 1, m, d, v, ub;
    int j;
    while (g < L && c[g] <= 3) g++;
    if (i == K - 1) {
        for (m = g; m > last && m > best; m--) {
            if (covers(c, m, g)) { best = m; break; }
        }
        return;
    }
    ub = g;
    for (j = i; j < K - 1 && ub < L; j++) ub = 3 * ub + 1;
    if (ub <= best) return;
    for (d = g; d > last; d--) {
        uint8_t *nc = c + L;
        memcpy(nc, c, (size_t)L);
        for (v = d; v < L; v++) {
            uint8_t t = (uint8_t)(nc[v - d] + 1);
            if (t < nc[v]) nc[v] = t;
        }
        dfs(i + 1, d);
    }
}

static long long solve(int k) {
    long long nb = (long long)(k + 3) * (k + 2) * (k + 1) / 6;
    long long v;
    K = k;
    L = 3 * nb + 4;
    best = 0;
    buf = (uint8_t *)malloc((size_t)(k + 1) * (size_t)L);
    buf[0] = 0;
    for (v = 1; v < L; v++) buf[v] = 4;
    dfs(0, 0);
    free(buf);
    return best;
}

static long long *memo = NULL;
static idx_t memo_len = 0;

seq_t a(idx_t n) {
    idx_t i;
    if (n < 1) return 0;
    if (n >= memo_len) {
        idx_t nl = n + 1;
        memo = (long long *)realloc(memo, sizeof(long long) * (size_t)nl);
        for (i = memo_len; i < nl; i++) memo[i] = -1;
        memo_len = nl;
    }
    if (memo[n] < 0) memo[n] = solve((int)n);
    return (seq_t)3 * (seq_t)memo[n];
}
