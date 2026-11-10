/*
 * A087721  Strictly increasing domain of A005185.
 * https://oeis.org/A087721
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   sieve. Compute the Hofstadter Q-sequence Q = A005185
 *            (Q(1)=Q(2)=1, Q(k) = Q(k-Q(k-1)) + Q(k-Q(k-2))). The terms are
 *            the distinct values Q(k) with Q(k-1) < Q(k), listed in
 *            increasing order
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Validated: partial, pass, 68 terms through n = 68 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>

static uint64_t *A087721_q = NULL;
static idx_t A087721_qlen = 0;          /* Q computed for indices 1..A087721_qlen */

static uint64_t *A087721_res = NULL;    /* sorted values for current window */
static idx_t A087721_rescnt = 0;
static idx_t A087721_resK = 0;

static int A087721_extend(idx_t K) {
    if (K <= A087721_qlen) return 1;
    uint64_t *p = (uint64_t *)realloc(A087721_q, (size_t)(K + 1) * sizeof(uint64_t));
    if (p == NULL) return 0;
    A087721_q = p;
    idx_t start = A087721_qlen + 1;
    if (A087721_qlen < 2) {
        A087721_q[0] = 0;
        A087721_q[1] = 1;
        A087721_q[2] = 1;
        start = 3;
    }
    for (idx_t i = start; i <= K; i++) {
        A087721_q[i] = A087721_q[i - A087721_q[i - 1]] + A087721_q[i - A087721_q[i - 2]];
    }
    A087721_qlen = K;
    return 1;
}

static int A087721_build(idx_t K) {
    if (!A087721_extend(K)) return 0;
    idx_t T = K / 5;   /* only values well below the Q(k) ~ k/2 band are trusted */
    unsigned char *mark = (unsigned char *)calloc((size_t)T + 1, 1);
    if (mark == NULL) return 0;
    for (idx_t k = 2; k <= K; k++) {
        uint64_t v = A087721_q[k];
        if (A087721_q[k - 1] < v && v < T) mark[v] = 1;
    }
    idx_t cnt = 0;
    for (idx_t v = 1; v < T; v++) if (mark[v]) cnt++;
    uint64_t *r = (uint64_t *)realloc(A087721_res, (size_t)(cnt + 1) * sizeof(uint64_t));
    if (r == NULL) { free(mark); return 0; }
    A087721_res = r;
    idx_t j = 0;
    for (idx_t v = 1; v < T; v++) if (mark[v]) A087721_res[j++] = (uint64_t)v;
    free(mark);
    A087721_rescnt = cnt;
    A087721_resK = K;
    return 1;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (A087721_resK == 0) {
        if (!A087721_build(4096)) return 0;
    }
    while (A087721_rescnt < n) {
        if (!A087721_build(A087721_resK * 2)) return 0;
    }
    return (seq_t)A087721_res[n - 1];
}
