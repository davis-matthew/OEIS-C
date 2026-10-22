/*
 * A005169  Number of fountains of n coins.
 * https://oeis.org/A005169
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = f(n,1), where f(n,p) = 0 if p > n, 1 if p = n,
 *            Sum_{q=1..p+1} f(n-p,q) if p < n (A168396). Only columns with
 *            p*(p-1)/2 <= n are reachable, so columns are capped at K; only
 *            the last K+1 rows of prefix sums are kept, in a ring buffer
 * Range:     u8 n <= 12, u16 n <= 22, u32 n <= 42, u64 n <= 82, u128 n <= 163,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *memo = NULL;
static idx_t memo_len = 0;

static void build(idx_t N) {
    idx_t K = 1;
    while (K * (K - 1) / 2 <= N) K++;
    idx_t R = K + 1, W = K + 1;
    seq_t *S = alloc_zero(R * W);
    seq_t *res = alloc_zero(N + 1);
    for (idx_t j = 0; j <= N; j++) {
        seq_t *row = S + (j % R) * W;
        row[0] = 0;
        for (idx_t p = 1; p <= K; p++) {
            seq_t f;
            if (p > j) f = 0;
            else if (p == j) f = 1;
            else {
                idx_t c = (p + 1 < K) ? p + 1 : K;
                f = S[((j - p) % R) * W + c];
            }
            row[p] = row[p - 1] + f;
        }
        if (j == 0) res[j] = 1;
        else res[j] = row[1];
    }
    release(S, R * W);
    if (memo) release(memo, memo_len);
    memo = res;
    memo_len = N + 1;
}

seq_t a(idx_t n) {
    if (n >= memo_len) {
        idx_t N = n;
        if (memo_len > 0 && 2 * memo_len > N) N = 2 * memo_len;
        build(N);
    }
    return memo[n];
}
