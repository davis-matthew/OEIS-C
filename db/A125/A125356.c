/*
 * A125356  Number of base 19 circular n-digit numbers with adjacent digits
 *          differing by 4 or less.
 * https://oeis.org/A125356
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) where A is the 19×19 adjacency matrix * with
 *            A[i][j] = 1 if |i−j| ≤ 4, otherwise 0. For n = 0 the * sequence
 *            is defined to be 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

#define D 19

/* Internal matrix type */
typedef struct {
    seq_t m[D][D];
} mat19;

/* Initialize a matrix to the identity */
static void mat_identity(mat19 *M) {
    for (int i = 0; i < D; ++i)
        for (int j = 0; j < D; ++j)
            M->m[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
}

/* Initialize adjacency matrix A (|i−j| ≤ 4 → 1, else 0) */
static void mat_base(mat19 *M) {
    for (int i = 0; i < D; ++i) {
        for (int j = 0; j < D; ++j) {
            int diff = i - j;
            if (diff < 0) diff = -diff;
            M->m[i][j] = (diff <= 4) ? (seq_t)1 : (seq_t)0;
        }
    }
}

/* Multiply two matrices: R = X * Y */
static void mat_mul(const mat19 *X, const mat19 *Y, mat19 *R) {
    for (int i = 0; i < D; ++i) {
        for (int j = 0; j < D; ++j) {
            seq_t sum = (seq_t)0;
            for (int k = 0; k < D; ++k)
                sum += X->m[i][k] * Y->m[k][j];
            R->m[i][j] = sum;
        }
    }
}

/* Compute a(n) */
seq_t a(idx_t n) {
    if (n == 0)
        return (seq_t)1;           /* by definition */

    mat19 result, power, temp;
    mat_identity(&result);
    mat_base(&power);

    idx_t exp = n;
    while (exp) {
        if (exp & (idx_t)1) {
            mat_mul(&result, &power, &temp);
            result = temp;
        }
        mat_mul(&power, &power, &temp);
        power = temp;
        exp >>= 1;
    }

    seq_t trace = (seq_t)0;
    for (int i = 0; i < D; ++i)
        trace += result.m[i][i];

    return trace;
}
