/*
 * A124784  Number of base 29 circular n-digit numbers with adjacent digits
 *          differing by 1 or less.
 * https://oeis.org/A124784
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) where A is the 29×29 adjacency matrix of a
 *            path graph with self‑loops: A[i][i] = 1, A[i][i±1] = 1 (0 ≤ i <
 *            29, bounds checked). This equals the sum Σ_{j=1}^{29}
 *            (1+2*cos(jπ/30))^n, which counts circular base‑29 numbers whose
 *            adjacent digits differ by at most 1
 * Range:     u8 n <= 3, u16 n <= 8, u32 n <= 19, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

#define K 29

/* multiply two K×K matrices: out = a * b */
static void matmul(seq_t out[K][K], const seq_t a[K][K], const seq_t b[K][K]) {
    seq_t temp[K][K];
    /* initialise temp to zero */
    for (idx_t i = 0; i < K; ++i)
        for (idx_t j = 0; j < K; ++j)
            temp[i][j] = 0;
    /* standard long multiplication */
    for (idx_t i = 0; i < K; ++i)
        for (idx_t k = 0; k < K; ++k) {
            seq_t aik = a[i][k];
            if (aik == 0) continue;
            for (idx_t j = 0; j < K; ++j)
                temp[i][j] += aik * b[k][j];
        }
    /* copy result to out */
    for (idx_t i = 0; i < K; ++i)
        for (idx_t j = 0; j < K; ++j)
            out[i][j] = temp[i][j];
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    /* Build adjacency matrix A */
    seq_t A[K][K], R[K][K];
    for (idx_t i = 0; i < (idx_t)K; ++i) {
        for (idx_t j = 0; j < (idx_t)K; ++j)
            A[i][j] = 0;
        A[i][i] = 1;
        if (i > 0)   A[i][i-1] = 1;
        if (i + 1 < (idx_t)K) A[i][i+1] = 1;
    }

    /* Initialise result matrix R as identity */
    for (idx_t i = 0; i < (idx_t)K; ++i)
        for (idx_t j = 0; j < (idx_t)K; ++j)
            R[i][j] = (i == j) ? 1 : 0;

    /* Binary exponentiation */
    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            matmul(R, R, A);
        }
        exp >>= 1;
        if (exp) {
            matmul(A, A, A);
        }
    }

    /* Compute trace: sum of diagonal entries */
    seq_t sum = 0;
    for (idx_t i = 0; i < (idx_t)K; ++i)
        sum += R[i][i];

    return sum;
}
