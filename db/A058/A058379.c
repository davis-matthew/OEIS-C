/*
 * A058379  Essentially parallel series-parallel networks with n labeled
 *          edges, multiple edges not allowed.
 * https://oeis.org/A058379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = sum_{m=1..n} (sum_{k=0..m-1} (m+k-1)! * sum_{j=0..k}
 *            sum_{i=0..j}
 *            ((-1)^i*2^i*Stirling2(m+j-i-1,j-i))/(i!*(m+j-i-1)!)/(k-j)!))
 *            *Stirling1(n,m)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 20,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Straightforward implementation of the recurrence derived from the functional equation
 * 2 A(x) = exp(A(x)) - 1 + log(1+x).
 * The recurrence for the coefficients a_n (n >= 2) is
 *   a_n = Σ_{k=1}^{n-1} C(n-1,k-1) * a_k * ( 2 a_{n-k} + (-1)^{n-k} (n-k-1)! )
 *          - (-1)^n (n-1)!,
 * with a_0 = 0, a_1 = 1.  The binomial coefficient C(n-1,k-1) and factorials are
 * computed using the helper functions binom and simple multiplication.  All
 * arithmetic is performed in the seq_t type which supports arbitrary-precision
 * integers.  The routine is O(n^2) in time and uses O(n) memory, which is
 * sufficient for n up to about 10,000.
 */
#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n) {
    /* base values */
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    /* allocate arrays for a[0..n] and factorial 0..(n-1) */
    seq_t *A = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    seq_t *fact = (seq_t*)malloc(n * sizeof(seq_t));
    if (!A || !fact) {
        /* memory allocation failed – return 0 as a safe fallback */
        if (A) free(A);
        if (fact) free(fact);
        return (seq_t)0;
    }

    /* initialize */
    A[0] = (seq_t)0;
    A[1] = (seq_t)1;
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i < n; ++i) {
        fact[i] = fact[i - 1] * (seq_t)i;
    }

    /* main loop */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t sum = (seq_t)0;
        for (idx_t k = 1; k < i; ++k) {
            /* binomial C(i-1, k-1) */
            seq_t bin = binom(i - 1, k - 1);
            /* term 1: 2 * a_k * a_{i-k} * bin */
            seq_t prod    = A[k] * A[i - k];
            prod          = prod * (seq_t)2 * bin;
            sum += prod;

            /* term 2: (-1)^{i-k} * a_k * fact[i-k-1] * bin */
            seq_t term = A[k] * fact[i - k - 1] * bin;
            if ((i - k) & 1) {          /* odd: sign negative */
                sum -= term;
            } else {                     /* even: sign positive */
                sum += term;
            }
        }

        /* subtract (-1)^i * fact[i-1] */
        if (i & 1) {                       /* i odd -> + fact */
            sum += fact[i - 1];
        } else {                           /* i even -> - fact */
            sum -= fact[i - 1];
        }

        A[i] = sum;
    }

    seq_t result = A[n];
    free(A);
    free(fact);
    return result;
}
