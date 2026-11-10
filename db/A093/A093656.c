/*
 * A093656  Row sums of lower triangular matrix A093654.
 * https://oeis.org/A093656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   row sums r_K[k] of A093654 (k = n-1, K = bit length of k) via
 *            u^0_t = 1; u^j_t = u^{j-1}_t if bit j-1 of k is 0, else u^j_t =
 *            sum_{s=t+1}^{2t} u^{j-1}_s + 2*u^{j-1}_{2t+1}; a(n) = u^K_0
 * Range:     u8 n <= 31, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Validated: partial, pass, 64 terms through n = 64 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;
    idx_t k = n - 1;
    unsigned K = 0;
    while (K < 64 && (k >> K) != 0) ++K;
    if (K == 0) return (seq_t)1;

    size_t T0 = ((size_t)1 << K) - 1;
    seq_t *cur = mem_new(seq_t, T0 + 2);
    seq_t *nxt = mem_new(seq_t, T0 + 2);
    seq_t *pre = mem_new(seq_t, T0 + 2);
    for (size_t t = 0; t <= T0; ++t) cur[t] = (seq_t)1;

    size_t Tprev = T0;
    for (unsigned j = 1; j <= K; ++j) {
        size_t Tj = (Tprev - 1) / 2;
        if ((k >> (j - 1)) & 1) {
            seq_t s = 0;
            for (size_t t = 0; t <= Tprev; ++t) { s += cur[t]; pre[t] = s; }
            for (size_t t = 0; t <= Tj; ++t) {
                seq_t v = pre[2 * t] - pre[t];
                v += (seq_t)2 * cur[2 * t + 1];
                nxt[t] = v;
            }
        } else {
            for (size_t t = 0; t <= Tj; ++t) nxt[t] = cur[t];
        }
        seq_t *tmp = cur; cur = nxt; nxt = tmp;
        Tprev = Tj;
    }
    seq_t res = cur[0];
    mem_free(cur);
    mem_free(nxt);
    mem_free(pre);
    return res;
}
