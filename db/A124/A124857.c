/*
 * A124857  Number of base 11 circular n-digit numbers with adjacent digits
 *          differing by 2 or less.
 * https://oeis.org/A124857
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = trace(A^n) for n ≥ 1, with A[i][j] = 1 if |i-j| ≤ 2, and
 *            a(0) = 1. A is the 11×11 adjacency matrix of the graph on digits
 *            0–10 with edges when the digit difference is ≤2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 28, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* External types supplied by the build. */
extern seq_t a(idx_t n);

/* Internals for the matrix exponentiation. */
static void mul_matrix(seq_t dst[11][11], const seq_t a[11][11], const seq_t b[11][11]) {
    seq_t tmp[11][11];
    for (int i = 0; i < 11; ++i) {
        for (int j = 0; j < 11; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < 11; ++k) {
                sum += a[i][k] * b[k][j];
            }
            tmp[i][j] = sum;
        }
    }
    for (int i = 0; i < 11; ++i)
        for (int j = 0; j < 11; ++j)
            dst[i][j] = tmp[i][j];
}

/* Compute A^n quickly and return its trace. */
static seq_t power_trace(idx_t n) {
    /* Base matrix A */
    static seq_t A[11][11];
    static int adj_inited = 0;
    if (!adj_inited) {
        for (int i = 0; i < 11; ++i) {
            for (int j = 0; j < 11; ++j) {
                A[i][j] = ((i + 2 >= j) && (i - 2 <= j)) ? (seq_t)1 : (seq_t)0;
            }
        }
        adj_inited = 1;
    }

    /* Result matrix (identity) */
    seq_t R[11][11];
    for (int i = 0; i < 11; ++i)
        for (int j = 0; j < 11; ++j)
            R[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;

    /* Working matrix B = A */
    seq_t B[11][11];
    for (int i = 0; i < 11; ++i)
        for (int j = 0; j < 11; ++j)
            B[i][j] = A[i][j];

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            /* R = R * B */
            mul_matrix(R, R, B);
        }
        /* B = B * B */
        mul_matrix(B, B, B);
        exp >>= 1;
    }

    /* Trace of R */
    seq_t tr = 0;
    for (int i = 0; i < 11; ++i)
        tr += R[i][i];
    return tr;
}

/* Public interface */
seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    return power_trace(n);
}
