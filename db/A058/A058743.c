/*
 * A058743  McKay-Thompson series of class 69A for Monster.
 * https://oeis.org/A058743
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [q^(n+1)] A*B - [n==0], where A = G(q^69)*G(q) +
 *            q^14*H(q^69)*H(q), B = G(q^23)*H(q^3) - q^4*H(q^23)*G(q^3); G =
 *            g.f. of A003114, H = g.f. of A003106
 * Range:     u8 n <= 42, u16 n <= 112, u32 n <= 341, u64 n <= 1138,
 *            u128 n <= 4057, big any n
 * Validated: standard, pass, 77 terms through n = 9319 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include "../utils/mem.h"

/* partitions of k into parts congruent to r1 or r2 mod 5, k = 0..N */
static seq_t *mk_part(idx_t N, int r1, int r2) {
    seq_t *dp = mem_new(seq_t, (size_t)(N + 1));
    idx_t part, i;
    dp[0] = 1;
    for (part = 1; part <= N; ++part) {
        int m = (int)(part % 5);
        if (m != r1 && m != r2) continue;
        for (i = part; i <= N; ++i) dp[i] += dp[i - part];
    }
    return dp;
}

seq_t a(idx_t n) {
    idx_t N = n + 1;
    idx_t i, j, k;
    seq_t *g = mk_part(N, 1, 4);
    seq_t *h = mk_part(N, 2, 3);
    seq_t *Ak = mem_new(seq_t, (size_t)(N + 1));
    seq_t *Bp = mem_new(seq_t, (size_t)(N + 1));
    seq_t *Bm = mem_new(seq_t, (size_t)(N + 1));
    seq_t P = 0, M = 0, res;

    for (k = 0; k <= N; ++k) {
        seq_t s = 0;
        for (j = 0; 69 * j <= k; ++j) s += g[j] * g[k - 69 * j];
        if (k >= 14) {
            for (j = 0; 69 * j <= k - 14; ++j) s += h[j] * h[k - 14 - 69 * j];
        }
        Ak[k] = s;
    }
    for (i = 0; 23 * i <= N; ++i) {
        for (j = 0; 23 * i + 3 * j <= N; ++j)
            Bp[23 * i + 3 * j] += g[i] * h[j];
        for (j = 0; 23 * i + 4 + 3 * j <= N; ++j)
            Bm[23 * i + 4 + 3 * j] += h[i] * g[j];
    }
    for (k = 0; k <= N; ++k) {
        P += Ak[k] * Bp[N - k];
        M += Ak[k] * Bm[N - k];
    }
    res = P - M;
    if (n == 0) res -= 1;

    mem_free(g);
    mem_free(h);
    mem_free(Ak);
    mem_free(Bp);
    mem_free(Bm);
    return res;
}
