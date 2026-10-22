/*
 * A000524  Number of rooted trees with n nodes, 2 of which are labeled.
 * https://oeis.org/A000524
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = [x^n] (D(x)^3 + 2*D(x)^2), where D(x) = B(x)/(1-B(x)) is
 *            the g.f. of A000107 and B(x) = Sum_{n>=1} A000081(n) x^n is the
 *            g.f. of rooted trees. A000081: b(1) = 1, b(m+1) = (1/m) *
 *            Sum_{k=1..m} (Sum_{d|k} d*b(d)) * b(m-k+1). D satisfies D =
 *            B*(1+D): d(n) = b(n) + Sum_{k=1..n-1} b(k)*d(n-k)....
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   zalloc_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/zalloc_arr.h"

static seq_t *B = 0, *C = 0, *D = 0, *D2 = 0, *D3 = 0;
static idx_t cap = 0; /* arrays are valid for indices 0..cap */

static void build(idx_t N) {
    if (B) { free(B); free(C); free(D); free(D2); free(D3); }
    B = zalloc_arr(N + 2);
    C = zalloc_arr(N + 2);
    D = zalloc_arr(N + 2);
    D2 = zalloc_arr(N + 2);
    D3 = zalloc_arr(N + 2);

    /* A000081 */
    B[1] = 1;
    for (idx_t m = 1; m < N; m++) {
        /* C[m] = sum_{d|m} d*B[d] */
        seq_t c = 0;
        for (idx_t d = 1; d * d <= m; d++) {
            if (m % d == 0) {
                idx_t e = m / d;
                c += (seq_t)d * B[d];
                if (e != d) c += (seq_t)e * B[e];
            }
        }
        C[m] = c;
        seq_t s = 0;
        for (idx_t k = 1; k <= m; k++) s += C[k] * B[m - k + 1];
        B[m + 1] = s / (seq_t)m;
    }

    /* D = B/(1-B) */
    for (idx_t i = 1; i <= N; i++) {
        seq_t s = B[i];
        for (idx_t k = 1; k < i; k++) s += B[k] * D[i - k];
        D[i] = s;
    }

    /* D^2 */
    for (idx_t i = 2; i <= N; i++) {
        seq_t s = 0;
        for (idx_t k = 1; k < i; k++) s += D[k] * D[i - k];
        D2[i] = s;
    }

    /* D^3 */
    for (idx_t i = 3; i <= N; i++) {
        seq_t s = 0;
        for (idx_t k = 1; k + 2 <= i; k++) s += D[k] * D2[i - k];
        D3[i] = s;
    }

    cap = N;
}

seq_t a(idx_t n) {
    if (n > cap) {
        idx_t N = cap * 2;
        if (N < n) N = n;
        if (N < 64) N = 64;
        build(N);
    }
    return D3[n] + 2 * D2[n];
}
