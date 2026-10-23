/*
 * A026134  a(n) = Sum_{k=1..n} T(k, k-1), where T is the array in A026120.
 * https://oeis.org/A026134
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = c_{n-1} – c_{n-2} where c_n = Σ_{i+j+k=n} M_i*M_j*M_k and
 *            M_i are the Motzkin numbers (M_0 = M_1 = 1, M_n = M_{n-1} +
 *            Σ_{k=0}^{n-2} M_k*M_{n-2-k}). Thus a(1)=1, a(2)=2, a(3)=6, …
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 22, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

extern seq_t a(idx_t n)
{
    /* Handle very small n quickly */
    if (n == 1)
        return (seq_t)1;

    /* Allocate arrays for Motzkin numbers (M), pairwise convolution (D),
       triple convolution (c) up to index n. */
    size_t N = (size_t)n + 1;
    seq_t *M = (seq_t *)malloc(N * sizeof(seq_t));
    seq_t *D = (seq_t *)malloc(N * sizeof(seq_t));
    seq_t *c = (seq_t *)malloc(N * sizeof(seq_t));
    if (!M || !D || !c) {
        /* Out of memory; return 0 as a safe default */
        if (M) free(M);
        if (D) free(D);
        if (c) free(c);
        return (seq_t)0;
    }

    /* Base Motzkin numbers */
    M[0] = (seq_t)1;
    M[1] = (seq_t)1;
    for (idx_t i = 2; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k <= i - 2; ++k) {
            sum += M[k] * M[i - 2 - k];
        }
        M[i] = M[i - 1] + sum;
    }

    /* Pairwise convolution D_t = Σ_{k=0}^t M_k * M_{t-k} */
    for (idx_t t = 0; t <= n; ++t) {
        seq_t sum = 0;
        for (idx_t k = 0; k <= t; ++k) {
            sum += M[k] * M[t - k];
        }
        D[t] = sum;
    }

    /* Triple convolution c_n = Σ_{i=0}^n M_i * D_{n-i} */
    for (idx_t i = 0; i <= n; ++i) {
        seq_t sum = 0;
        for (idx_t k = 0; k <= i; ++k) {
            sum += M[k] * D[i - k];
        }
        c[i] = sum;
    }

    /* Compute a(n) using a(n) = c_{n-1} - c_{n-2} for n >= 2 */
    seq_t result;
    if (n == 1) {
        result = c[0];
    } else {
        result = c[n - 1] - c[n - 2];
    }

    /* Free allocated memory */
    free(M);
    free(D);
    free(c);

    return result;
}
