/*
 * A000444  Number of partially labeled rooted trees with n nodes (3 of which
 *          are labeled).
 * https://oeis.org/A000444
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. A(x) = B^3*(9-8B+2B^2)/(1-B)^5, B = g.f. of A000081. With C
 *            = 1/(1-B) and E = C - 1 = B*C, this equals 9E^3 + 10E^4 + 3E^5
 *            (all coefficients nonnegative). A000081: r(1) = 1, r(m) =
 *            (1/(m-1)) * Sum_{k=1..m-1} (Sum_{d|k} d*r(d)) * r(m-k). C(0) =
 *            1, C(m) = Sum_{k=1..m} r(k)*C(m-k)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 37, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *R = 0, *S = 0, *C = 0, *E2 = 0, *E3 = 0, *A = 0;
static idx_t done = 0, cap = 0;

static seq_t *grow_arr(seq_t *old, idx_t oldcap, idx_t newcap) {
    seq_t *p = (seq_t *)calloc((size_t)newcap, sizeof(seq_t));
    idx_t i;
    for (i = 0; i < newcap; i++) p[i] = 0;
    for (i = 0; i < oldcap; i++) p[i] = old[i];
    if (old) free(old);
    return p;
}

static void ensure(idx_t n) {
    idx_t m, i, k;
    if (n + 1 > cap) {
        idx_t nc = cap ? cap : 64;
        while (nc < n + 1) nc *= 2;
        R = grow_arr(R, cap, nc);
        S = grow_arr(S, cap, nc);
        C = grow_arr(C, cap, nc);
        E2 = grow_arr(E2, cap, nc);
        E3 = grow_arr(E3, cap, nc);
        A = grow_arr(A, cap, nc);
        cap = nc;
        C[0] = 1;
    }
    for (m = done + 1; m <= n; m++) {
        seq_t t;
        /* A000081 */
        if (m == 1) {
            R[1] = 1;
        } else {
            t = 0;
            for (k = 1; k < m; k++) t += S[k] * R[m - k];
            R[m] = t / (seq_t)(m - 1);
        }
        t = 0;
        for (k = 1; k * k <= m; k++) {
            if (m % k == 0) {
                idx_t q = m / k;
                t += (seq_t)k * R[k];
                if (q != k) t += (seq_t)q * R[q];
            }
        }
        S[m] = t;
        /* C = 1/(1-B) */
        t = 0;
        for (k = 1; k <= m; k++) t += R[k] * C[m - k];
        C[m] = t;
        /* E2 = E^2 */
        t = 0;
        for (i = 1; 2 * i < m; i++) t += C[i] * C[m - i];
        t *= 2;
        if (m % 2 == 0 && m >= 2) t += C[m / 2] * C[m / 2];
        E2[m] = t;
        /* E3 = E^3 */
        t = 0;
        for (i = 2; i + 1 <= m; i++) t += E2[i] * C[m - i];
        E3[m] = t;
        /* A = 9E^3 + 10E^4 + 3E^5 */
        {
            seq_t s4 = 0, s5 = 0;
            for (i = 3; i + 1 <= m; i++) s4 += E3[i] * C[m - i];
            for (i = 3; i + 2 <= m; i++) s5 += E3[i] * E2[m - i];
            A[m] = 9 * E3[m] + 10 * s4 + 3 * s5;
        }
        done = m;
    }
}

seq_t a(idx_t n) {
    if (n < 3) return 0;
    ensure(n);
    return A[n];
}
