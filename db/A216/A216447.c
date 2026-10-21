/*
 * A216447  G.f. satisfies: A(x) = (1 + x*(1-x)*A(x)^2) * (1 + x^2*A(x)^2).
 * https://oeis.org/A216447
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coefficient of x^n in A(x) where A(x) satisfies A(x) = (1
 *            + x*(1-x)*A(x)^2) * (1 + x^2*A(x)^2). This leads to the
 *            recurrence a(0) = 1 for n ≥ 1: a(n) = C[n-1] + D[n-3] - D[n-4]
 *            where C[k] = coeff of x^k in A(x)^2 and D[k] = coeff of x^k in
 *            A(x)^4. C[0] = D[0] = 1. C and D are computed iter...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* The external types seq_t and idx_t are provided by the build. */
seq_t a(idx_t n)
{
    /* Allocate arrays for a, C, and D with size n+1 elements. */
    seq_t *A = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    seq_t *C = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    seq_t *D = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));

    if (!A || !C || !D) {
        /* In case of allocation failure, return 0. */
        return (seq_t)0;
    }

    /* Initial conditions. */
    A[0] = 1;   /* a(0) = 1 */
    C[0] = 1;   /* coeff of x^0 in A(x)^2 is a0^2 = 1 */
    D[0] = 1;   /* coeff of x^0 in A(x)^4 is a0^4 = 1 */

    /* Compute terms up to n. */
    for (idx_t k = 1; k <= n; ++k) {
        /* Compute a[k] = C[k-1] + D[k-3] - D[k-4] (ignoring negative indices). */
        seq_t ak = 0;
        if (k >= 1) ak += C[k-1];
        if (k >= 3) ak += D[k-3];
        if (k >= 4) ak -= D[k-4];
        A[k] = ak;

        /* Compute C[k] = Σ_{i=0..k} a[i]*a[k-i]. */
        seq_t sumC = 0;
        for (idx_t i = 0; i <= k; ++i) {
            sumC += A[i] * A[k - i];
        }
        C[k] = sumC;

        /* Compute D[k] = Σ_{i=0..k} C[i]*C[k-i]. */
        seq_t sumD = 0;
        for (idx_t i = 0; i <= k; ++i) {
            sumD += C[i] * C[k - i];
        }
        D[k] = sumD;
    }

    /* Retrieve the requested term. */
    seq_t result = A[n];

    /* Free allocated memory. */
    free(A);
    free(C);
    free(D);

    return result;
}
