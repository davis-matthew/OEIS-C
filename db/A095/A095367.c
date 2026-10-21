/*
 * A095367  Number of walks of length n between two nodes at distance 2 in the
 *          cycle graph C_9.
 * https://oeis.org/A095367
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of walks of length n between two vertices at
 *            distance 2 in the cycle graph C₉. It is computed as the (0,2)
 *            entry of the adjacency matrix of C₉ raised to the n‑th power
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: The adjacency matrix contains only 0 and 1, so all
   intermediate computations use only addition and multiplication,
   avoiding negative intermediate values. */

#include <stdbool.h>
#include <stdint.h>

/* 9 × 9 adjacency matrix type */
typedef struct {
    seq_t m[9][9];
} Mat;

/* Identity matrix */
static Mat identity(void) {
    Mat I = {0};
    for (int i = 0; i < 9; ++i)
        I.m[i][i] = 1;
    return I;
}

/* Matrix multiplication: C = A * B */
static Mat mul_mat(const Mat *A, const Mat *B) {
    Mat C = {0};
    for (int i = 0; i < 9; ++i) {
        for (int k = 0; k < 9; ++k) {
            seq_t aik = A->m[i][k];
            if (aik == 0)
                continue;
            for (int j = 0; j < 9; ++j) {
                C.m[i][j] += aik * B->m[k][j];
            }
        }
    }
    return C;
}

/* Fast exponentiation of adjacency matrix */
static Mat pow_mat(idx_t exp, const Mat *base) {
    Mat result = identity();
    Mat cur = *base;
    while (exp > 0) {
        if (exp & 1)
            result = mul_mat(&result, &cur);
        exp >>= 1;
        if (exp)
            cur = mul_mat(&cur, &cur);
    }
    return result;
}

/* Build the adjacency matrix for C9 once */
static const Mat *adjacency_matrix(void) {
    static bool initialized = false;
    static Mat adj = {0};
    if (!initialized) {
        /* vertices 0..8, edges between i and (i±1) mod 9 */
        for (int i = 0; i < 9; ++i) {
            adj.m[i][(i + 1) % 9] = 1;
            adj.m[i][(i + 8) % 9] = 1;  /* (i-1) mod 9 */
        }
        initialized = true;
    }
    return &adj;
}

/* Main sequence function */
seq_t a(idx_t n) {
    /* Compute the (0,2) entry of A^n where A is the adjacency matrix. */
    const Mat *adj = adjacency_matrix();
    Mat P = pow_mat(n, adj);
    return P.m[0][2];
}
