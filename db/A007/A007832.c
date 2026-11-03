/*
 * A007832  Number of point labeled 5,6-free two-graphs with n nodes.
 * https://oeis.org/A007832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [n odd] + Sum_{k=2..n} (-1)^(n-k) * Stirling2(n,k) *
 *            (k+1)^(k-2) (Stirling transform of A007831)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 11, u64 n <= 18, u128 n <= 29,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 374 (2026-11-03)
 * Helpers:   ipow_idx, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/ipow_idx.h"
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static void release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

seq_t a(idx_t n)
{
    idx_t i, k;
    seq_t *S;
    seq_t pos = 0, neg = 0;

    if (n < 1) return 0;

    /* Row n of the Stirling numbers of the second kind: S[k] = S2(n,k). */
    S = alloc_zero(n + 1);
    S[0] = 1;
    for (i = 1; i <= n; i++) {
        for (k = i; k >= 1; k--) {
            S[k] = S[k] * (seq_t)k + S[k - 1];
        }
        S[0] = 0;
    }

    if (n & 1) pos += 1;
    for (k = 2; k <= n; k++) {
        seq_t t = ipow_idx((seq_t)(k + 1), k - 2) * S[k];
        if (((n - k) & 1) == 0) pos += t;
        else neg += t;
    }
    release(S, n + 1);
    return pos - neg;
}
