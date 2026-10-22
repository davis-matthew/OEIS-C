/*
 * A002028  Number of connected graphs on n labeled nodes, each node being
 *          colored with one of 3 colors, such that no edge joins nodes of the
 *          same color.
 * https://oeis.org/A002028
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = 1; a(n) = A(n) - Sum_{k=1..n-1}
 *            binomial(n-1,k-1)*a(k)*A(n-k), where A(n) = A191371(n) =
 *            Sum_{k=0..n} binomial(n,k)*2^(k*(n-k))*T(n-k) and T(m) =
 *            A047863(m) = Sum_{j=0..m} binomial(m,j)*2^(j*(m-j)) (logarithmic
 *            transform of A191371)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 11, u128 n <= 17,
 *            big any n
 * Helpers:   alloc_a2028, p2_a2028
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_a2028.h"
#include "../utils/p2_a2028.h"

static seq_t *memo_c = NULL;
static idx_t memo_len = 0;

static void build_a2028(idx_t N) {
    idx_t m, k;
    seq_t *A = alloc_a2028(N + 1);
    seq_t *T = alloc_a2028(N + 1);
    seq_t *c = alloc_a2028(N + 1);
    seq_t *prev = alloc_a2028(N + 2);
    seq_t *cur = alloc_a2028(N + 2);
    seq_t *tmp;

    A[0] = 1; T[0] = 1; c[0] = 1;
    prev[0] = 1;
    for (m = 1; m <= N; m++) {
        seq_t s, t;
        cur[0] = 1;
        for (k = 1; k < m; k++) cur[k] = prev[k - 1] + prev[k];
        cur[m] = 1;

        s = 0;
        for (k = 0; k <= m; k++) s += cur[k] * p2_a2028(k * (m - k));
        T[m] = s;

        s = 0;
        for (k = 0; k <= m; k++) s += cur[k] * p2_a2028(k * (m - k)) * T[m - k];
        A[m] = s;

        t = A[m];
        for (k = 1; k < m; k++) t -= prev[k - 1] * c[k] * A[m - k];
        c[m] = t;

        tmp = prev; prev = cur; cur = tmp;
    }

    free(A); free(T); free(prev); free(cur);
    if (memo_c) free(memo_c);
    memo_c = c;
    memo_len = N + 1;
}

seq_t a(idx_t n) {
    if (n >= memo_len) {
        idx_t N = n;
        if (memo_len > 0 && N < 2 * memo_len) N = 2 * memo_len;
        if (N < 16) N = 16;
        build_a2028(N);
    }
    return memo_c[n];
}
