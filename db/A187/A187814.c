/*
 * A187814  G.f. A(x) satisfies: 1/A(x)^2 + 4*x*A(x)^2 = 1/A(x^2) +
 *          2*x*A(x^2).
 * https://oeis.org/A187814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the functional equation 1/A(x)^2 + 4*x*A(x)^2 =
 *            1/A(x^2) + 2*x*A(x^2) where A(x) = Σ a(n) x^n, a(0)=1 The
 *            implementation uses only integer operations and runs in O(n^2)
 *            time, which is well below the limit for n ≈ 10 000
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 20, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Return a(n) of OEIS A187814.   idx_t and seq_t are provided by the build. */
seq_t a(idx_t n)
{
    /* allocate arrays a, c (coefficients of A^2), q (1/A^2), r (1/A(x^2)) */
    size_t len = (size_t)n + 1;
    seq_t *A = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *C = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *Q = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *R = (seq_t *)calloc(len, sizeof(seq_t));

    if (!A || !C || !Q || !R)
    {
        /* fall back to zero if allocation fails */
        return 0;
    }

    /* initial conditions  */
    A[0] = 1;          /* a(0) = 1 */
    C[0] = 1;          /* A(x)^2 coefficient at 0 is 1^2 = 1 */
    Q[0] = 1;          /* (1/A(x)^2) coefficient at 0 is 1 */
    R[0] = 1;          /* (1/A(x^2)) coefficient at 0 is 1 */

    /* precompute reciprocals for powers of 2 up to n (unused directly) */

    for (idx_t k = 1; k <= n; ++k)
    {
        /* ----------------------------------------------------- */
        /* 1. Compute r[k]  (reciprocal of A(x^2))                */
        seq_t sum_r = 0;
        for (idx_t j = 1; j <= k >> 1; ++j)   /* j <= floor(k/2) */
            sum_r += A[j] * R[k - (j << 1)];
        R[k] = -sum_r;

        /* ----------------------------------------------------- */
        /* 2. Prepare required sums for a[k] computation          */
        /*    c_except  = Σ_{i=1}^{k-1} a[i]*a[k-i]               */
        seq_t c_except = 0;
        seq_t B_k      = 0; /* Σ_{i=1}^{k-1} c[i]*Q[k-i]                */
        for (idx_t i = 1; i < k; ++i)
        {
            c_except += A[i] * A[k - i];
            B_k      += C[i] * Q[k - i];
        }

        /* ----------------------------------------------------- */
        /* 3. Compute the right-hand side of the functional eq.  */
        seq_t rhs = R[k];
        if (k & 1)          /* k is odd: add 2*a[(k-1)/2] */
            rhs += A[(k - 1) >> 1] + A[(k - 1) >> 1];

        /* ----------------------------------------------------- */
        /* 4. Solve for a[k] using the derived linear relation  */
        seq_t numerator = (seq_t)4 * C[k - 1] - rhs - c_except - B_k;
        /* It is guaranteed that `numerator` is even. */
        A[k] = numerator / 2;

        /* ----------------------------------------------------- */
        /* 5. Update c[k] = Σ a[i]a[k-i]   (include 2*a[k])      */
        C[k] = c_except + (A[k] + A[k]);

        /* ----------------------------------------------------- */
        /* 6. Compute Q[k] = -(c[k] + B_k) (1/A(x)^2)            */
        Q[k] = -(C[k] + B_k);
    }

    seq_t result = A[n];
    free(A); free(C); free(Q); free(R);
    return result;
}
