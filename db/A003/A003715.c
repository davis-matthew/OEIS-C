/*
 * A003715  Expansion of e.g.f. sin(sin(sin(x))) (odd powers only).
 * https://oeis.org/A003715
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   e.g.f. sin(sin(sin(x))), a(n) = (2n+1)! [x^(2n+1)]. Computed by
 *            applying sin three times to a series u(x), starting from u = x.
 *            For S = sin(u) and C = cos(u), with E.g.f. coefficients: S' =
 *            C*u' and C' = -S*u', S(0) = 0, C(0) = 1, which gives S(m+1) =
 *            Sum_{k=0..m} binomial(m,k)*C(k)*u(m-k+1), C(m...
 * Range:     i8 n <= 2, i16 n <= 4, i32 n <= 6, i64 n <= 11, i128 n <= 18,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/alloc_zero.h"
#include "../utils/release.h"

seq_t a(idx_t n) {
    idx_t N = 2 * n + 2;
    seq_t *u = alloc_zero(N);
    seq_t *S = alloc_zero(N);
    seq_t *C = alloc_zero(N);
    seq_t *row = alloc_zero(N);
    idx_t i, m, k;
    int level;
    seq_t result;

    u[1] = 1;
    for (level = 0; level < 3; level++) {
        for (i = 0; i < N; i++) row[i] = 0;
        row[0] = 1;
        S[0] = 0;
        C[0] = 1;
        for (m = 0; m + 1 < N; m++) {
            seq_t s = 0, c = 0;
            for (k = 0; k <= m; k++) {
                seq_t t = row[k] * u[m - k + 1];
                s += t * C[k];
                c -= t * S[k];
            }
            S[m + 1] = s;
            C[m + 1] = c;
            for (k = m + 1; k >= 1; k--) row[k] += row[k - 1];
        }
        for (i = 0; i < N; i++) u[i] = S[i];
    }
    result = u[2 * n + 1];

    release(u, N);
    release(S, N);
    release(C, N);
    release(row, N);
    return result;
}
