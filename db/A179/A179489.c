/*
 * A179489  G.f.: A(x) = x/(1 - (1-3x)*A( x/(1-3x) )).
 * https://oeis.org/A179489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the (1,1)-entry of M^(n-1) where * M has main diagonal
 *            3*i-2, 1 in the first column, * 1 in the superdiagonal and 0
 *            elsewhere. * The following algorithm computes a(n) via the
 *            recurrence * derived from the functional equation * A(x) = x /
 *            (1 - (1-3x)*A(x/(1-3x))). * It uses only integer ar...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* The types seq_t and idx_t are defined externally. */
extern seq_t a(idx_t n);

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;

    size_t N = (size_t)n;
    /* Allocate working arrays */
    seq_t *A = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *B = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    seq_t *POW3 = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!A || !B || !POW3) {
        /* Allocation failed – free what we have and return 0 */
        free(A); free(B); free(POW3);
        return (seq_t)0;
    }

    /* Helper: pre‑compute powers of 3 */
    POW3[0] = (seq_t)1;
    for (size_t i = 1; i <= N; ++i) POW3[i] = POW3[i - 1] * (seq_t)3;

    /* Constants */
    A[0] = (seq_t)0;
    B[0] = (seq_t)0;
    A[1] = (seq_t)1;                     /* a(1) = 1 */

    /* Initialise B[j] with contributions from a(1) */
    for (size_t j = 1; j <= N; ++j) {
        /* binom(j-1,0)=1 and 3^{j-1} */
        B[j] = POW3[j - 1];
    }

    /* Main loop: compute a(2)..a(N) */
    for (size_t m = 2; m <= N; ++m) {
        seq_t sum1 = (seq_t)0;
        seq_t sum2 = (seq_t)0;

        /* sum1 = Σ_{j=1}^{m-1} A[m-j] * B[j]  */
        for (size_t j = 1; j <= m - 1; ++j)
            sum1 += A[m - j] * B[j];

        /* sum2 = Σ_{j=1}^{m-2} A[m-1-j] * B[j]  */
        for (size_t j = 1; j <= m - 2; ++j)
            sum2 += A[m - 1 - j] * B[j];

        /* a[m] = sum1 - 3*sum2 */
        A[m] = sum1 - (seq_t)3 * sum2;

        /* Update B[t] for t >= m   (add influence of a[m])  */
        seq_t cur_binom = (seq_t)1; /* binom(m-1, m-1) */
        for (size_t t = m; t <= N; ++t) {
            seq_t term = A[m] * cur_binom * POW3[t - m];
            B[t] += term;
            /* prepare binom(t, m-1) for next t */
            if (t < N)
                cur_binom = cur_binom * ((seq_t)t) /
                            ((seq_t)(t - m + 1));
        }
    }

    seq_t result = A[N];
    free(A); free(B); free(POW3);
    return result;
}
