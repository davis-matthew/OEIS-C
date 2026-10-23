/*
 * A125364  Number of base 27 circular n-digit numbers with adjacent digits
 *          differing by 4 or less.
 * https://oeis.org/A125364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = tr(A^n), where A[i][j] = 1 if |i - j| <= 4 (digits
 *            0..26), else 0. Base case: a(0) = 1 (empty circular number).
 *            This counts circular n‑digit numbers in base 27 whose adjacent
 *            digits differ by at most 4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define DIM 27

/* Matrix multiplication: C = A * B  (27 x 27, type seq_t) */
static void mat_mul(seq_t (*C)[DIM], const seq_t (*A)[DIM], const seq_t (*B)[DIM]) {
    for (idx_t i = 0; i < DIM; ++i) {
        for (idx_t j = 0; j < DIM; ++j) {
            seq_t sum = 0;
            for (idx_t k = 0; k < DIM; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

/* Matrix exponentiation: res = base^exp */
static void mat_pow(seq_t (*res)[DIM], const seq_t (*base)[DIM], idx_t exp) {
    /* Initialize res as identity matrix */
    seq_t identity[DIM][DIM] = {0};
    for (idx_t i = 0; i < DIM; ++i) identity[i][i] = 1;
    seq_t cur[DIM][DIM];
    for (idx_t i = 0; i < DIM; ++i)
        for (idx_t j = 0; j < DIM; ++j)
            cur[i][j] = base[i][j];

    for (idx_t i = 0; i < DIM; ++i)
        for (idx_t j = 0; j < DIM; ++j)
            res[i][j] = identity[i][j];

    while (exp) {
        if (exp & 1) {
            seq_t temp[DIM][DIM];
            mat_mul(temp, res, cur);
            for (idx_t i = 0; i < DIM; ++i)
                for (idx_t j = 0; j < DIM; ++j)
                    res[i][j] = temp[i][j];
        }
        seq_t temp[DIM][DIM];
        mat_mul(temp, cur, cur);
        for (idx_t i = 0; i < DIM; ++i)
            for (idx_t j = 0; j < DIM; ++j)
                cur[i][j] = temp[i][j];
        exp >>= 1;
    }
}

/* Main sequence function */
seq_t a(idx_t n) {
    if (n == 0) return 1;  /* empty circular number */

    /* Build adjacency matrix A */
    seq_t A[DIM][DIM];
    for (idx_t i = 0; i < DIM; ++i) {
        for (idx_t j = 0; j < DIM; ++j) {
            seq_t diff = (i > j) ? (i - j) : (j - i);
            A[i][j] = (diff <= 4) ? 1 : 0;
        }
    }

    /* Compute A^n */
    seq_t An[DIM][DIM];
    mat_pow(An, A, n);

    /* Sum diagonal entries to get the trace */
    seq_t result = 0;
    for (idx_t i = 0; i < DIM; ++i) result += An[i][i];
    return result;
}
