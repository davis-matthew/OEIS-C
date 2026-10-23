/*
 * A125435  Number of base 23 circular n-digit numbers with adjacent digits
 *          differing by 7 or less.
 * https://oeis.org/A125435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n), where A is the 23×23 adjacency matrix with
 *            A[i][j] = 1 if |i-j| ≤ 7, otherwise 0. For n = 0 the sequence
 *            defines a(0) = 1. This implementation uses fast exponentiation
 *            of the matrix and works for any seq_t (fixed‐size integer or
 *            arbitrary‐precision class)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#define NODES 23

/* Multiply two NODES×NODES matrices: C = A * B */
static void mat_mul(const seq_t A[NODES][NODES],
                    const seq_t B[NODES][NODES],
                    seq_t C[NODES][NODES]) {
    for (int i = 0; i < NODES; ++i) {
        for (int j = 0; j < NODES; ++j) {
            seq_t sum = (seq_t)0;
            for (int k = 0; k < NODES; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

/* Copy matrix src into dst */
static void mat_copy(const seq_t src[NODES][NODES],
                     seq_t dst[NODES][NODES]) {
    for (int i = 0; i < NODES; ++i)
        for (int j = 0; j < NODES; ++j)
            dst[i][j] = src[i][j];
}

/* Compute the OEIS sequence A125435 */
seq_t a(idx_t n) {
    if (n == (idx_t)0) return (seq_t)1;

    /* Build the adjacency matrix once */
    static seq_t adjacency[NODES][NODES];
    static bool adj_initialized = false;
    if (!adj_initialized) {
        for (int i = 0; i < NODES; ++i) {
            for (int j = 0; j < NODES; ++j) {
                int diff = i > j ? i - j : j - i;
                adjacency[i][j] = (diff <= 7) ? (seq_t)1 : (seq_t)0;
            }
        }
        adj_initialized = true;
    }

    /* Matrices for exponentiation */
    seq_t result[NODES][NODES];
    seq_t power[NODES][NODES];
    seq_t temp[NODES][NODES];

    /* Identity matrix for result */
    for (int i = 0; i < NODES; ++i) {
        for (int j = 0; j < NODES; ++j) {
            result[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
        }
    }

    /* Start with adjacency matrix */
    mat_copy(adjacency, power);

    idx_t exp = n;
    while (exp > 0) {
        if (exp & (idx_t)1) {
            mat_mul(result, power, temp);
            mat_copy(temp, result);
        }
        mat_mul(power, power, temp);
        mat_copy(temp, power);
        exp >>= 1;
    }

    /* Return the trace of result */
    seq_t total = (seq_t)0;
    for (int i = 0; i < NODES; ++i)
        total += result[i][i];
    return total;
}
