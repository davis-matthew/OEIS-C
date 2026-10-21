/*
 * A223143  G.f. satisfies: A(x)^3 = A(x^2)^3 + 9*x.
 * https://oeis.org/A223143
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) is given by the generating function * A(x) = (1 +
 *            Sum_{k>=0} 9*x^{2^k})^{1/3}. * The sequence satisfies A(x)^3 =
 *            A(x^2)^3 + 9*x. * The recurrence used below derives from the
 *            coefficient‑wise * identity: * S[n] = sum_{i+j+k=n} a_i a_j a_k
 *            * S[n] = (n%2==0 ? S[n/2] : 0) + (n==1?9:0) * and the...
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 12, i64 n <= 23, i128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required standard header for malloc/realloc. */
#include <stdlib.h>

/* The function a computes the nth term of OEIS sequence A223143.
 * It memoizes all computed terms so that subsequent calls are
 * efficient, even for non‑sequential n. */
seq_t a(idx_t n)
{
    /* Static storage for memoization. */
    static seq_t *A   = NULL;   /* a[0..] */
    static seq_t *B   = NULL;   /* B[s] = sum_{i=0}^s a[i]*a[s-i] */
    static seq_t *S   = NULL;   /* S[s] = sum_{i+j+k=s} a_i*a_j*a_k   */
    static size_t  cap = 0;     /* current allocated size */
    static idx_t   max_n = (idx_t)-1;   /* highest computed index */

    /* Ensure we have storage for at least n+1 elements. */
    if (n >= (idx_t)cap) {
        size_t newcap = cap ? cap << 1 : 16;
        while (newcap <= (size_t)n) newcap <<= 1;

        A = (seq_t*)realloc(A, newcap * sizeof(seq_t));
        B = (seq_t*)realloc(B, newcap * sizeof(seq_t));
        S = (seq_t*)realloc(S, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* First call: initialise the base case. */
    if (max_n == (idx_t)-1) {
        A[0] = 1;            /* a(0) = 1 */
        B[0] = 1;            /* B[0] = a0*a0 */
        S[0] = 1;            /* S[0] = a0^3 = 1 */
        max_n = 0;
    }

    /* Return immediately if already computed. */
    if (n <= max_n)
        return A[(size_t)n];

    /* Compute values up to n. */
    for (idx_t k = max_n + 1; k <= n; ++k)
    {
        /* S[k] = (k%2==0 ? S[k/2] : 0) + (k==1?9:0) */
        seq_t Sk;
        if (k == 1)
            Sk = 9;
        else if ((k & 1) == 0)
            Sk = S[(size_t)(k / 2)];
        else
            Sk = 0;
        S[(size_t)k] = Sk;

        /* sumProd = sum_{i=1}^{k-1} a[i]*a[k-i] */
        seq_t sumProd = 0;
        for (idx_t i = 1; i < k; ++i)
            sumProd += A[(size_t)i] * A[(size_t)(k - i)];

        /* sum3 = sum_{t=1}^{k-1} a[t]*B[k-t] */
        seq_t sum3 = 0;
        for (idx_t t = 1; t < k; ++t)
            sum3 += A[(size_t)t] * B[(size_t)(k - t)];

        /* Compute a[k] */
        seq_t numerator = Sk - sumProd - sum3;
        A[(size_t)k] = numerator / 3;   /* division is exact */

        /* Compute B[k] = 2*a[k] + sum_{i=1}^{k-1} a[i]*a[k-i] */
        B[(size_t)k] = (2 * A[(size_t)k]) + sumProd;
    }

    max_n = n;
    return A[(size_t)n];
}
