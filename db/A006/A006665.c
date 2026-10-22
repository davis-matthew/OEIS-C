/*
 * A006665  G.f.: { ( Product_{j=1..infinity} (1-x^j) - 1 )/x }^24.
 * https://oeis.org/A006665
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   G.f. = ((Product_{j>=1} (1-x^j) - 1)/x)^24, where Product_{j>=1}
 *            (1-x^j) = Sum_{k} (-1)^k x^(k(3k-1)/2) (Euler's pentagonal
 *            theorem); computed by multiplying 1 by the sparse series 24
 *            times
 * Range:     i8 n <= 1, i16 n <= 4, i32 n <= 25, i64 n <= 27, i128 n <= 27,
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

static void compute(idx_t N) {
    idx_t L = N + 1;
    size_t cap = 16, cnt = 0;
    idx_t *pos = (idx_t *)malloc(cap * sizeof(idx_t));
    int *sgn = (int *)malloc(cap * sizeof(int));
    for (idx_t k = 1;; k++) {
        idx_t e1 = k * (3 * k - 1) / 2;
        idx_t e2 = k * (3 * k + 1) / 2;
        int s = (k & 1) ? -1 : 1;
        if (e1 - 1 > N) break;
        if (cnt + 2 > cap) {
            cap *= 2;
            pos = (idx_t *)realloc(pos, cap * sizeof(idx_t));
            sgn = (int *)realloc(sgn, cap * sizeof(int));
        }
        pos[cnt] = e1 - 1; sgn[cnt] = s; cnt++;
        if (e2 - 1 <= N) { pos[cnt] = e2 - 1; sgn[cnt] = s; cnt++; }
    }
    seq_t *g = alloc_zero(L);
    g[0] = 1;
    for (int r = 0; r < 24; r++) {
        seq_t *h = alloc_zero(L);
        for (size_t t = 0; t < cnt; t++) {
            idx_t d = pos[t];
            if (sgn[t] > 0) {
                for (idx_t i = 0; i + d <= N; i++) h[i + d] += g[i];
            } else {
                for (idx_t i = 0; i + d <= N; i++) h[i + d] -= g[i];
            }
        }
        release(g, L);
        g = h;
    }
    free(pos);
    free(sgn);
    if (memo) release(memo, memo_len);
    memo = g;
    memo_len = L;
}

seq_t a(idx_t n) {
    if (n >= memo_len) {
        idx_t N = n;
        if (memo_len > 0 && N < 2 * memo_len) N = 2 * memo_len;
        if (N < 64) N = 64;
        compute(N);
    }
    return memo[n];
}
