/*
 * A140045  Sequence generated from Z/6Z addition table considered as a
 *          matrix.
 * https://oeis.org/A140045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = term (1,2) of X^n, where X is the Z/6Z addition table
 *            considered as a 6×6 integer matrix. The matrix is X = [0 1 2 3 4
 *            5; 1 2 3 4 5 0; 2 3 4 5 0 1; 3 4 5 0 1 2; 4 5 0 1 2 3; 5 0 1 2 3
 *            4]; a(n) is computed by raising X to the nth power and reading
 *            element (0,1) (indices are 0‑based, so (1,2...
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static void mat_mul(seq_t R[6][6], const seq_t A[6][6], const seq_t B[6][6]) {
    seq_t T[6][6] = {0};
    for (int i = 0; i < 6; ++i) {
        for (int k = 0; k < 6; ++k) {
            seq_t aik = A[i][k];
            for (int j = 0; j < 6; ++j) {
                T[i][j] += aik * B[k][j];
            }
        }
    }
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 6; ++j)
            R[i][j] = T[i][j];
}

static void mat_pow(seq_t R[6][6], const seq_t A[6][6], idx_t exp) {
    seq_t res[6][6] = {0};
    seq_t B[6][6];
    seq_t Temp[6][6];

    /* Initialise res = I, B = A */
    for (int i = 0; i < 6; ++i) {
        for (int j = 0; j < 6; ++j) {
            res[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
            B[i][j]   = A[i][j];
        }
    }

    while (exp > 0) {
        if (exp & 1) {
            mat_mul(Temp, res, B);
            for (int i = 0; i < 6; ++i)
                for (int j = 0; j < 6; ++j)
                    res[i][j] = Temp[i][j];
        }
        mat_mul(Temp, B, B);
        for (int i = 0; i < 6; ++i)
            for (int j = 0; j < 6; ++j)
                B[i][j] = Temp[i][j];
        exp >>= 1;
    }
    for (int i = 0; i < 6; ++i)
        for (int j = 0; j < 6; ++j)
            R[i][j] = res[i][j];
}

seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)0;      /* not part of the sequence, but harmless */

    static const seq_t X[6][6] = {
        {0, 1, 2, 3, 4, 5},
        {1, 2, 3, 4, 5, 0},
        {2, 3, 4, 5, 0, 1},
        {3, 4, 5, 0, 1, 2},
        {4, 5, 0, 1, 2, 3},
        {5, 0, 1, 2, 3, 4}
    };

    seq_t M[6][6];
    mat_pow(M, X, n);

    return M[0][1];   /* (1,2) in OEIS 1‑based indexing */
}
