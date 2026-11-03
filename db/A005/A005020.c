/*
 * A005020  The number of n X n (0,1)-matrices with a 1-width of 2.
 * https://oeis.org/A005020
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, more
 * Formula:   a(n) = (2^(n^2) - P(n)) - Sum_{k=1..n}
 *            (-1)^(k+1)*C(n,k)*2^(n*(n-k)), where the 1-width is the fewest
 *            columns that together cover every row. Let D_j be the set of
 *            rows that have a 0 in column j. Then width <= 2 means some pair
 *            D_i, D_j is disjoint, and width 1 means some D_j is empty. P(n)
 *            = numb...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Validated: partial, pass, 6 terms through n = 6 (2026-11-03)
 * Helpers:   binomial, mem, pow2_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"
#include "../utils/pow2_u64.h"
#include "../utils/mem.h"

static void a005020_rec(const uint64_t *adj, int W, uint64_t *buf, int depth,
                        int n, uint64_t *cnt) {
    const uint64_t *cand = buf + (size_t)depth * W;
    uint64_t *next = buf + (size_t)(depth + 1) * W;
    for (int w = 0; w < W; w++) {
        uint64_t bits = cand[w];
        while (bits) {
            int b = __builtin_ctzll(bits);
            bits &= bits - 1;
            int v = w * 64 + b;
            cnt[depth + 1]++;
            if (depth + 1 == n) continue;
            const uint64_t *av = adj + (size_t)v * W;
            uint64_t pc = 0;
            int any = 0;
            for (int x = 0; x < W; x++) {
                uint64_t m;
                if (x < w) m = 0;
                else if (x == w) m = (b == 63) ? 0 : (cand[x] & av[x] & (~0ULL << (b + 1)));
                else m = cand[x] & av[x];
                next[x] = m;
                if (m) { any = 1; pc += (uint64_t)__builtin_popcountll(m); }
            }
            if (!any) continue;
            if (depth + 2 == n) cnt[n] += pc;
            else a005020_rec(adj, W, buf, depth + 1, n, cnt);
        }
    }
}

seq_t a(idx_t n) {
    int nn = (int)n;
    uint64_t N = 1ULL << nn;
    int W = (int)((N + 63) / 64);
    uint64_t *adj = (uint64_t *)calloc((size_t)N * W, sizeof(uint64_t));
    uint64_t *buf = (uint64_t *)calloc((size_t)(nn + 2) * W, sizeof(uint64_t));
    uint64_t *cnt = (uint64_t *)calloc((size_t)nn + 2, sizeof(uint64_t));
    for (uint64_t u = 1; u < N; u++)
        for (uint64_t v = 1; v < N; v++)
            if (u & v) adj[u * W + v / 64] |= 1ULL << (v % 64);
    for (uint64_t v = 1; v < N; v++) buf[v / 64] |= 1ULL << (v % 64);
    a005020_rec(adj, W, buf, 0, nn, cnt);

    /* Stirling numbers of the second kind S2(n,k) */
    seq_t *S = mem_new(seq_t, (size_t)(nn + 1) * (nn + 1));
    for (int i = 0; i <= nn; i++)
        for (int k = 0; k <= nn; k++) S[i * (nn + 1) + k] = 0;
    S[0] = 1;
    for (int i = 1; i <= nn; i++)
        for (int k = 1; k <= i; k++)
            S[i * (nn + 1) + k] = S[(i - 1) * (nn + 1) + k - 1]
                                + (seq_t)k * S[(i - 1) * (nn + 1) + k];

    seq_t P = 0, fact = 1;
    for (int k = 1; k <= nn; k++) {
        fact *= (seq_t)k;
        P += (seq_t)cnt[k] * fact * S[nn * (nn + 1) + k];
    }

    seq_t pos = 0, neg = 0;
    for (int k = 1; k <= nn; k++) {
        seq_t t = binomial(n, (idx_t)k) * pow2_u64((uint64_t)nn * (uint64_t)(nn - k));
        if (k & 1) pos += t; else neg += t;
    }
    seq_t B1 = pos - neg;

    seq_t res = pow2_u64((uint64_t)nn * (uint64_t)nn) - P - B1;
    free(adj); free(buf); free(cnt); mem_free(S);
    return res;
}
