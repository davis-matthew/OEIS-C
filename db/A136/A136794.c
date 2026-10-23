/*
 * A136794  Number of unlabeled marked rooted trees with n nodes.
 * https://oeis.org/A136794
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2 * f(n), where f(n) satisfies * * f(1) = 1 * f(n) =
 *            (4/(n-1)) * sum_{k=1}^{n-1} ( sum_{d|k} d * f(d) ) * f(n-k) * *
 *            This recurrence comes from the generating function T(x) = 2x *
 *            exp(2*sum_{k>=1}T(x^k)/k) * for marked rooted trees with node
 *            and edge marks + or -
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;  // below offset, return 0 (irrelevant)

    /* Allocate arrays for f[1..n] and C[1..n] */
    size_t sz = (size_t)(n + 1U);
    seq_t *f = (seq_t*)malloc(sz * sizeof(seq_t));
    seq_t *C = (seq_t*)malloc(sz * sizeof(seq_t));
    if (!f || !C) {
        /* allocation failed; abort (caller should handle appropriately) */
        if (f) free(f);
        if (C) free(C);
        return 0;
    }

    /* Initialize to zero */
    for (idx_t i = 0; i <= n; ++i) {
        f[i] = 0;
        C[i] = 0;
    }

    /* Base case */
    f[1] = (seq_t)1;
    /* Initialize divisor sums for all k with divisor 1 */
    for (idx_t m = 1; m <= n; ++m) {
        C[m] += (seq_t)1 * f[1];
    }

    /* Compute f[2..n] */
    for (idx_t i = 2; i <= n; ++i) {
        seq_t S = 0;
        /* sum_{k=1}^{i-1} C[k] * f[i-k] */
        for (idx_t k = 1; k < i; ++k) {
            S += C[k] * f[i - k];
        }
        /* f[i] = 4/(i-1) * S */
        f[i] = (seq_t)4 * S / (seq_t)(i - 1);

        /* Update divisor sums C[m] for multiples of i */
        for (idx_t m = i; m <= n; m += i) {
            C[m] += (seq_t)i * f[i];
        }
    }

    seq_t result = 2 * f[n];

    free(f);
    free(C);

    return result;
}
