/*
 * A004309  Hit polynomials, coefficient of y^2 in N_n(y).
 * https://oeis.org/A004309
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k=2..n} (-1)^k * C(k,2) * r_k * (n-k)!, where r_k is
 *            the number of ways to place k non-attacking rooks on the
 *            circular board {(i, i+j mod n) : 0 <= j <= 3} (so a(n) counts
 *            permutations with exactly 2 hits on that board); r_k is computed
 *            by a transfer-matrix DP over the rows
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 22,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

seq_t a(idx_t n)
{
    idx_t N = n + 1;
    idx_t s, i, j, d, k;
    int st, c;
    seq_t *r = alloc_zero(N);
    seq_t *cur = alloc_zero(8 * N);
    seq_t *nxt = alloc_zero(8 * N);
    seq_t *tmp;
    seq_t pos = 0, neg = 0, f = 1;

    for (s = 0; s < 8; s++) {
        for (j = 0; j < 8 * N; j++) cur[j] = 0;
        cur[s * N] = 1;
        for (i = 0; i < n; i++) {
            for (j = 0; j < 8 * N; j++) nxt[j] = 0;
            for (st = 0; st < 8; st++) {
                for (d = 0; d <= i && d < N; d++) {
                    seq_t v = cur[(idx_t)st * N + d];
                    if (v == 0) continue;
                    nxt[(idx_t)(st >> 1) * N + d] += v;
                    if (d + 1 < N) {
                        for (c = 0; c < 4; c++) {
                            if (!((st >> c) & 1)) {
                                int o = st | (1 << c);
                                nxt[(idx_t)(o >> 1) * N + d + 1] += v;
                            }
                        }
                    }
                }
            }
            tmp = cur; cur = nxt; nxt = tmp;
        }
        for (d = 0; d < N; d++) r[d] += cur[s * N + d];
    }

    for (k = n; k >= 2; k--) {
        seq_t term = r[k] * f * (seq_t)(k * (k - 1) / 2);
        if (k % 2 == 0) pos += term; else neg += term;
        f *= (seq_t)(n - k + 1);
    }

    release(r, N);
    release(cur, 8 * N);
    release(nxt, 8 * N);
    return pos - neg;
}
