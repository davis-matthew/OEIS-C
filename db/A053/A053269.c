/*
 * A053269  Coefficients of the '6th-order' mock theta function psi(q).
 * https://oeis.org/A053269
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 38, i16 n <= 120, i32 n <= 402, i64 n <= 1432,
 *            i128 n <= 5322, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*   Sequence A053269 – coefficients of the 6th‑order mock theta function ψ(q)
 *   ψ(q) = Σ_{n≥0} (-1)^n q^{(n+1)^2} (1-q)(1-q^3)…(1-q^{2n-1})
 *          / (1+q)(1+q^2)…(1+q^{2n+1})
 *   For each k the factor contributes to the series as a polynomial
 *   A_k(q) = Π_{j=0}^{k-1}(1-q^{2j+1}) / Π_{j=0}^{k}(1+q^{j+1}),
 *   shifted by (k+1)^2 with sign (-1)^k.
 *   The implementation builds A_k incrementally, updating it in
 *   O(n) time per k; the number of k's is O(√n).
 */

#include <stdlib.h> /* malloc, calloc, free */

seq_t a(idx_t n)
{
    /* Allocate coefficients for the current A_k polynomial */
    idx_t N = n;
    seq_t *A = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!A)
        return (seq_t)0;          /* allocation failure – return 0 */

    /* A_0(q) = 1/(1+q) = Σ_{i≥0} (-1)^i q^i  (up to degree N) */
    for (idx_t i = 0; i <= N; ++i)
        A[i] = ((i & 1) ? (seq_t)-1 : (seq_t)1);

    /* Determine maximal k with (k+1)^2 ≤ N */
    idx_t maxK = 0;
    while ((maxK + 1) * (maxK + 1) <= N)
        ++maxK;

    seq_t result = (seq_t)0;

    for (idx_t k = 0; k <= maxK; ++k)
    {
        /* contribution of k‑th summand */
        idx_t shift = (k + 1) * (k + 1);
        if (shift <= N)
        {
            idx_t j = N - shift;          /* coefficient index in A_k */
            seq_t sign = ((k & 1) ? (seq_t)-1 : (seq_t)1);
            result += sign * A[j];
        }

        /* No need to update A after the last iteration */
        if (k == maxK)
            break;

        /* 1. multiply by (1 – q^{2k+1}) (adds one odd factor to numerator) */
        idx_t m1 = 2 * k + 1;
        for (long long i = N; i >= (long long)m1; --i)
            A[(idx_t)i] -= A[(idx_t)(i - m1)];

        /* 2. divide by (1 + q^{2k+2})  (adds one even factor to denominator) */
        idx_t m2 = 2 * k + 2;
        for (idx_t i = m2; i <= N; ++i)
            A[i] -= A[i - m2];

        /* 3. divide by (1 + q^{2k+3})  (adds one odd factor to denominator) */
        idx_t m3 = 2 * k + 3;
        for (idx_t i = m3; i <= N; ++i)
            A[i] -= A[i - m3];
    }

    free(A);
    return result;
}
