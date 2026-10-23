/*
 * A125358  Number of base 21 circular n-digit numbers with adjacent digits
 *          differing by 4 or less.
 * https://oeis.org/A125358
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) where A is the 21×21 adjacency matrix * with
 *            A[i][j] = 1 if |i-j| ≤ 4, otherwise 0. * a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#define B 21

/* Initialize the adjacency matrix */
static void init_base_matrix(seq_t M[B][B]) {
    for (size_t i = 0; i < B; ++i) {
        for (size_t j = 0; j < B; ++j) {
            size_t diff = i > j ? i - j : j - i;
            M[i][j] = (diff <= 4) ? (seq_t)1 : (seq_t)0;
        }
    }
}

/* Copy matrix src into dest */
static void matrix_copy(seq_t dst[B][B], const seq_t src[B][B]) {
    for (size_t i = 0; i < B; ++i)
        for (size_t j = 0; j < B; ++j)
            dst[i][j] = src[i][j];
}

/* Multiply matrices A and B, store result in R (all size B×B) */
static void matrix_mul(const seq_t A[B][B], const seq_t Bm[B][B], seq_t R[B][B]) {
    for (size_t i = 0; i < B; ++i) {
        for (size_t j = 0; j < B; ++j) {
            seq_t sum = 0;
            for (size_t k = 0; k < B; ++k) {
                sum += A[i][k] * Bm[k][j];
            }
            R[i][j] = sum;
        }
    }
}

/* Compute the n-th term */
seq_t a(idx_t n) {
    /* offset 0 */
    if (n == 0) return (seq_t)1;

    static bool base_inited = false;
    static seq_t base_matrix[B][B];
    if (!base_inited) {
        init_base_matrix(base_matrix);
        base_inited = true;
    }

    seq_t res[B][B], power[B][B], temp[B][B];

    /* identity matrix for the result */
    for (size_t i = 0; i < B; ++i)
        for (size_t j = 0; j < B; ++j)
            res[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;

    matrix_copy(power, base_matrix);

    idx_t exp = n;
    while (exp) {
        if (exp & (idx_t)1) {
            matrix_mul(res, power, temp);
            matrix_copy(res, temp);
        }
        matrix_mul(power, power, temp);
        matrix_copy(power, temp);
        exp >>= 1;
    }

    /* trace of M^n gives the required count */
    seq_t trace = 0;
    for (size_t i = 0; i < B; ++i)
        trace += res[i][i];

    return trace;
}
