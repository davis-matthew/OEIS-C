/*
 * A124714  Number of base 21 circular n-digit numbers with adjacent digits
 *          differing by 1 or less.
 * https://oeis.org/A124714
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(Aⁿ) where A is a 21×21 matrix with A[i][i] = 1 and
 *            A[i][i−1] = A[i][i+1] = 1 when within bounds. a(0)=1. The matrix
 *            represents the adjacency of a path of length 21 with self‑loops
 *            (adjacent digits may differ by 0 or 1). The trace of Aⁿ counts
 *            the number of closed walks of length n, i...
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* multiply two 21×21 matrices: C = A * B  */
static void mat_mult(seq_t C[21][21], const seq_t A[21][21], const seq_t B[21][21]) {
    seq_t tmp[21][21];
    for (idx_t i = 0; i < 21; ++i) {
        for (idx_t j = 0; j < 21; ++j) {
            seq_t sum = 0;
            for (idx_t k = 0; k < 21; ++k) {
                sum += A[i][k] * B[k][j];
            }
            tmp[i][j] = sum;
        }
    }
    for (idx_t i = 0; i < 21; ++i)
        for (idx_t j = 0; j < 21; ++j)
            C[i][j] = tmp[i][j];
}

/* compute matrix power: R = Baseⁿ  */
static void mat_pow(seq_t R[21][21], const seq_t Base[21][21], idx_t n) {
    /* initialise R as identity */
    for (idx_t i = 0; i < 21; ++i)
        for (idx_t j = 0; j < 21; ++j)
            R[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;

    seq_t temp[21][21];
    for (idx_t i = 0; i < 21; ++i)
        for (idx_t j = 0; j < 21; ++j)
            temp[i][j] = Base[i][j];

    while (n) {
        if (n & 1)
            mat_mult(R, R, temp);
        mat_mult(temp, temp, temp);
        n >>= 1;
    }
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;

    /* constant adjacency matrix for the path with self‑loops */
    static seq_t A[21][21];
    static bool init = false;
    if (!init) {
        init = true;
        for (idx_t i = 0; i < 21; ++i) {
            for (idx_t j = 0; j < 21; ++j)
                A[i][j] = 0;
            A[i][i] = (seq_t)1;
            if (i > 0)     A[i][i - 1] = (seq_t)1;
            if (i + 1 < 21) A[i][i + 1] = (seq_t)1;
        }
    }

    seq_t M[21][21];
    mat_pow(M, A, n);

    seq_t tr = 0;
    for (idx_t i = 0; i < 21; ++i)
        tr += M[i][i];
    return tr;
}
