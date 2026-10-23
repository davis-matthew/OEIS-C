/*
 * A198299  L.g.f.: Sum_{n>=1} (x^n/n) / Product_{d|n} (1 - d*x^n).
 * https://oeis.org/A198299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * [x^n] L(x) where L(x) = Sum_{k>=1}
 *            (x^k/k)/Product_{d|k}(1-d*x^k)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 53, u64 n <= 113,
 *            u128 n <= 233, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the OEIS A198299 sequence. */

#include <stdlib.h>

/* Assuming seq_t and idx_t are defined by the build system. */

seq_t a(idx_t n) {
    /* Return 0 for non-positive indices – not used but safe. */
    if (n < 1) return (seq_t)0;

    /* Allocate array for accumulating results up to n. */
    seq_t *A = (seq_t*)calloc((size_t)n + 1, sizeof(seq_t));
    if (!A) return (seq_t)0;  /* Allocation failed. */

    /* Temporary array for h values – maximum size needed is n. */
    /* We'll reuse the same buffer for each outer loop iteration. */
    seq_t *h = (seq_t*)malloc((size_t)n * sizeof(seq_t));
    if (!h) { free(A); return (seq_t)0; }

    /* Buffer for divisors – maximum number of divisors <= 200 for n <= 10000. */
    int divs[256];
    int dcnt;

    for (idx_t k = 1; k <= n; ++k) {
        /* Max t such that (t+1)*k <= n  =>  t <= n/k - 1 */
        idx_t max_t = n / k;
        if (max_t == 0) continue;          /* No multiples of k are <= n */
        --max_t;                            /* Now max_t is maximum t (t >=0) */
        if (max_t > n) max_t = n;           /* Safety, though not needed */

        /* Initialize h[0..max_t] */
        for (idx_t t = 0; t <= max_t; ++t) h[t] = (seq_t)0;
        h[0] = (seq_t)1;

        /* Compute divisors of k */
        dcnt = 0;
        for (idx_t d = 1; d*d <= k; ++d) {
            if (k % d == 0) {
                divs[dcnt++] = (int)d;
                if (d != k / d) divs[dcnt++] = (int)(k / d);
            }
        }

        /* Unbounded knapsack DP: h[t] = coefficient of z^t in
           Product_{d|k} 1/(1 - d*z)  */
        for (int ii = 0; ii < dcnt; ++ii) {
            seq_t div = (seq_t)divs[ii];
            for (idx_t t = 1; t <= max_t; ++t) {
                h[t] += div * h[t - 1];
            }
        }

        /* Accumulate contributions to a[m] */
        for (idx_t t = 0; t <= max_t; ++t) {
            idx_t m = k * (t + 1);
            if (m > n) continue;  /* Should not happen due to max_t definition */
            A[m] += (seq_t)(t + 1) * h[t];
        }
    }

    seq_t result = A[n];
    free(h);
    free(A);
    return result;
}
