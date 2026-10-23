/*
 * A125433  Number of base 21 circular n-digit numbers with adjacent digits
 *          differing by 7 or less.
 * https://oeis.org/A125433
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n), where A[i][j] = 1 if |i-j| ≤ 7, 0 otherwise.
 *            a(0)=1, a(n)=∑(A^n)[i][i] for n≥1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define M 21

/* Multiply two M×M matrices over seq_t. */
static void mul_matrix(seq_t C[M][M], const seq_t A[M][M], const seq_t B[M][M]) {
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < M; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < M; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return 1;

    /* Adjacency matrix A, built once. */
    static seq_t base[M][M];
    static bool init = false;
    if (!init) {
        init = true;
        for (int i = 0; i < M; ++i) {
            for (int j = 0; j < M; ++j) {
                base[i][j] = ((i > j) ? (i - j) : (j - i)) <= 7 ? 1 : 0;
            }
        }
    }

    /* P = I (identity), B = base. */
    seq_t P[M][M], B[M][M];
    for (int i = 0; i < M; ++i) {
        for (int j = 0; j < M; ++j) {
            P[i][j] = (i == j) ? 1 : 0;
            B[i][j] = base[i][j];
        }
    }

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            seq_t temp[M][M];
            mul_matrix(temp, P, B);
            for (int i = 0; i < M; ++i)
                for (int j = 0; j < M; ++j)
                    P[i][j] = temp[i][j];
        }
        exp >>= 1;
        if (exp) {
            seq_t temp[M][M];
            mul_matrix(temp, B, B);
            for (int i = 0; i < M; ++i)
                for (int j = 0; j < M; ++j)
                    B[i][j] = temp[i][j];
        }
    }

    /* trace of P = A^n. */
    seq_t result = 0;
    for (int i = 0; i < M; ++i)
        result += P[i][i];

    return result;
}
