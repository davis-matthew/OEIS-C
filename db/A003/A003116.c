/*
 * A003116  Expansion of the reciprocal of the g.f. defining A039924.
 * https://oeis.org/A003116
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   G.f. 1/D(x), D(x) = Sum_{k>=0} (-1)^k
 *            x^(k^2)/Product_{i=1..k}(1-x^i); D = P - Q with P,Q >= 0, a(0) =
 *            1, a(j) = Sum_{i=1..j} Q[i]*a(j-i) - Sum_{i=1..j} P[i]*a(j-i)
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 40, u64 n <= 80, u128 n <= 161,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *A3116_P = NULL, *A3116_Q = NULL, *A3116_A = NULL;
static idx_t A3116_cap = 0;

static void A3116_build(idx_t m) {
    seq_t *P = (seq_t *)calloc(m + 1, sizeof(seq_t));
    seq_t *Q = (seq_t *)calloc(m + 1, sizeof(seq_t));
    seq_t *den = (seq_t *)calloc(m + 1, sizeof(seq_t));
    seq_t *A = (seq_t *)calloc(m + 1, sizeof(seq_t));
    idx_t i, j, k;
    for (j = 0; j <= m; j++) { P[j] = 0; Q[j] = 0; den[j] = 0; A[j] = 0; }
    den[0] = 1;
    P[0] = 1;
    for (k = 1; k * k <= m; k++) {
        idx_t kk = k * k;
        for (j = k; j <= m; j++) den[j] += den[j - k];
        for (j = kk; j <= m; j++) {
            if (k & 1) Q[j] += den[j - kk];
            else P[j] += den[j - kk];
        }
    }
    for (j = 1; j <= m; j++) {
        if (P[j] >= Q[j]) { P[j] -= Q[j]; Q[j] = 0; }
        else { Q[j] -= P[j]; P[j] = 0; }
    }
    A[0] = 1;
    for (j = 1; j <= m; j++) {
        seq_t plus = 0, minus = 0;
        for (i = 1; i <= j; i++) {
            if (Q[i] != 0) plus += Q[i] * A[j - i];
            else if (P[i] != 0) minus += P[i] * A[j - i];
        }
        A[j] = plus - minus;
    }
    A3116_P = P;
    A3116_Q = Q;
    A3116_A = A;
    A3116_cap = m + 1;
}

seq_t a(idx_t n) {
    if (n >= A3116_cap) {
        idx_t m = n;
        if (m < 64) m = 64;
        if (A3116_cap > 0 && m < 2 * A3116_cap) m = 2 * A3116_cap;
        A3116_build(m);
    }
    return A3116_A[n];
}
