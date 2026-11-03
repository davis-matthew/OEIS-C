/*
 * A002465  Number of ways to place n nonattacking bishops on an n X n board.
 * https://oeis.org/A002465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Sum_{k=0..n} W(n,k)*B(n,n-k), where W(n,k) and B(n,k)
 *            count the ways to place k nonattacking rooks on the Ferrers
 *            boards formed by the sorted diagonal lengths of the two bishop
 *            colors (diagonals x+y even or odd) of the n X n board; a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 26,
 *            big any n
 * Validated: standard, pass, 34 terms through n = 370 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

static int a002465_cmp_ll(const void *p, const void *q) {
    long long x = *(const long long *)p, y = *(const long long *)q;
    return (x > y) - (x < y);
}

static seq_t a002465_combine(const seq_t *W, const seq_t *B, long long N) {
    seq_t sum = 0;
    long long k;
    for (k = 0; k <= N; k++) sum += W[k] * B[N - k];
    return sum;
}

#define A002465_CAP 1024

/* dp must point to N+1 seq_t objects that already exist */
static void a002465_rook(seq_t *dp, long long N, int parity) {
    long long *h = (long long *)malloc((size_t)(2 * N + 1) * sizeof(long long));
    long long cnt = 0, s, i, j;
    for (j = 0; j <= N; j++) dp[j] = 0;
    dp[0] = 1;
    for (s = parity; s <= 2 * N - 2; s += 2) {
        long long m = s < 2 * N - 2 - s ? s : 2 * N - 2 - s;
        h[cnt++] = m + 1;
    }
    qsort(h, (size_t)cnt, sizeof(long long), a002465_cmp_ll);
    for (i = 0; i < cnt; i++) {
        long long top = i + 1 < N ? i + 1 : N;
        for (j = top; j >= 1; j--) {
            if (j - 1 < h[i]) {
                dp[j] += dp[j - 1] * (seq_t)(uint64_t)(h[i] - (j - 1));
            }
        }
    }
    free(h);
}

seq_t a(idx_t n) {
    long long N = (long long)n;
    if (n == 0) return 1;
    if (N < A002465_CAP) {
        seq_t W[A002465_CAP], B[A002465_CAP];
        a002465_rook(W, N, 0);
        a002465_rook(B, N, 1);
        return a002465_combine(W, B, N);
    } else {
        seq_t *W = mem_new(seq_t, (size_t)(N + 1));
        seq_t *B = mem_new(seq_t, (size_t)(N + 1));
        seq_t r;
        a002465_rook(W, N, 0);
        a002465_rook(B, N, 1);
        r = a002465_combine(W, B, N);
        return r;
    }
}
