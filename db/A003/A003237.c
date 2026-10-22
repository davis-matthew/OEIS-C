/*
 * A003237  Number of partially achiral planted trees with n nodes.
 * https://oeis.org/A003237
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. A(x) = x*G(x)/(x-G(x)) with G(x) = sum_{k>=1}
 *            A000081(k)*x^(2k). Equivalently A = G + A*G/x, so a(i) = [i
 *            even] A000081(i/2) + Sum_{k>=1, 2k-1<=i} A000081(k)*a(i-2k+1),
 *            a(0)=a(1)=0. A000081: r(1)=1, r(m+1) = (1/m) Sum_{k=1..m}
 *            s(k)*r(m+1-k), s(k) = Sum_{d|k} d*r(d)
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 39, u64 n <= 76, u128 n <= 149,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *A3237_R = 0;
static seq_t *A3237_S = 0;
static seq_t *A3237_A = 0;
static idx_t A3237_cap = 0;

static void A3237_build(idx_t N) {
    idx_t M = N / 2 + 3;
    idx_t m, k, j, i;
    seq_t *R = (seq_t *)calloc(M + 1, sizeof(seq_t));
    seq_t *S = (seq_t *)calloc(M + 1, sizeof(seq_t));
    seq_t *A = (seq_t *)calloc(N + 1, sizeof(seq_t));
    for (m = 0; m <= M; m++) { R[m] = (seq_t)0; S[m] = (seq_t)0; }
    R[1] = (seq_t)1;
    for (m = 1; m < M; m++) {
        /* add contributions of d = m to s(j) for multiples j of m */
        seq_t t = (seq_t)m * R[m];
        for (j = m; j <= M; j += m) S[j] += t;
        seq_t sum = (seq_t)0;
        for (k = 1; k <= m; k++) sum += S[k] * R[m + 1 - k];
        R[m + 1] = sum / (seq_t)m;
    }
    for (i = 0; i <= N; i++) {
        seq_t v = (seq_t)0;
        if (i >= 2) {
            if (i % 2 == 0) v = R[i / 2];
            for (k = 1; 2 * k - 1 <= i; k++) v += R[k] * A[i - 2 * k + 1];
        }
        A[i] = v;
    }
    if (A3237_R) free(A3237_R);
    if (A3237_S) free(A3237_S);
    if (A3237_A) free(A3237_A);
    A3237_R = R;
    A3237_S = S;
    A3237_A = A;
    A3237_cap = N + 1;
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)0;
    if (n >= A3237_cap) {
        idx_t N = n;
        if (A3237_cap > 0 && N < 2 * A3237_cap) N = 2 * A3237_cap;
        if (N < 64) N = 64;
        A3237_build(N);
    }
    return A3237_A[n];
}
