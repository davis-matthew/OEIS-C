/*
 * A001524  Number of stacks, or arrangements of n pennies in contiguous rows,
 *          each touching 2 in row below.
 * https://oeis.org/A001524
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   G.f.: 1 + Sum_{m>=1} q^(m*(m+1)/2) / (Prod_{k=1..m-1} (1-q^k)^2
 *            * (1-q^m))
 * Range:     u8 n <= 15, u16 n <= 39, u32 n <= 111, u64 n <= 355,
 *            u128 n <= 1230, big any n
 * Helpers:   zalloc_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/zalloc_arr.h"

static seq_t *memo = NULL;
static idx_t memo_len = 0;

static void build(idx_t N) {
    seq_t *R = zalloc_arr(N + 1);
    seq_t *F = zalloc_arr(N + 1);
    seq_t *G = zalloc_arr(N + 1);
    idx_t m, i;
    int c;
    F[0] = 1;
    R[0] = 1;
    for (m = 1; m * (m + 1) / 2 <= N; m++) {
        idx_t T = m * (m + 1) / 2;
        idx_t L = N - T;
        for (i = 0; i <= L; i++) G[i] = F[i];
        for (i = m; i <= L; i++) G[i] += G[i - m];
        for (i = 0; i <= L; i++) R[i + T] += G[i];
        for (c = 0; c < 2; c++)
            for (i = m; i <= L; i++) F[i] += F[i - m];
    }
    memo = R;
    memo_len = N + 1;
}

seq_t a(idx_t n) {
    if (memo == NULL || n >= memo_len) {
        idx_t N = n;
        if (memo_len * 2 > N) N = memo_len * 2;
        if (N < 64) N = 64;
        build(N);
    }
    return memo[n];
}
