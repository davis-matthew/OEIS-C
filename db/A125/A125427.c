/*
 * A125427  Number of base 15 circular n-digit numbers with adjacent digits
 *          differing by 7 or less.
 * https://oeis.org/A125427
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n), where A[i][j] = 1 if |i−j| ≤ 7, else 0. a(0)
 *            = 1. For n >= 1 the answer is the trace of n-th power of the
 *            15×15 adjacency matrix
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#ifndef DIGIT_COUNT
#define DIGIT_COUNT 15
#endif

/* Global adjacency matrix for base‑15 digits. */
static seq_t A[DIGIT_COUNT][DIGIT_COUNT];
static int initialized = 0;

/* Initialise A once. */
static void init_matrices(void)
{
    if (initialized) return;
    for (int i = 0; i < DIGIT_COUNT; ++i) {
        for (int j = 0; j < DIGIT_COUNT; ++j) {
            int diff = (i > j) ? (i - j) : (j - i);
            A[i][j] = (diff <= 7) ? (seq_t)1 : (seq_t)0;
        }
    }
    initialized = 1;
}

/* Multiply two 15×15 matrices: C = X * Y. */
static void mat_mul(seq_t C[DIGIT_COUNT][DIGIT_COUNT],
                    const seq_t X[DIGIT_COUNT][DIGIT_COUNT],
                    const seq_t Y[DIGIT_COUNT][DIGIT_COUNT])
{
    seq_t temp[DIGIT_COUNT][DIGIT_COUNT];
    for (int i = 0; i < DIGIT_COUNT; ++i) {
        for (int j = 0; j < DIGIT_COUNT; ++j) {
            seq_t sum = (seq_t)0;
            for (int k = 0; k < DIGIT_COUNT; ++k) {
                sum += X[i][k] * Y[k][j];
            }
            temp[i][j] = sum;
        }
    }
    /* Copy back to C. */
    for (int i = 0; i < DIGIT_COUNT; ++i)
        for (int j = 0; j < DIGIT_COUNT; ++j)
            C[i][j] = temp[i][j];
}

/* Compute the trace of a 15×15 matrix. */
static seq_t mat_trace(const seq_t M[DIGIT_COUNT][DIGIT_COUNT])
{
    seq_t trace = (seq_t)0;
    for (int i = 0; i < DIGIT_COUNT; ++i) {
        trace += M[i][i];
    }
    return trace;
}

/* OEIS sequence A125427. */
seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    init_matrices();

    /* Result starts as the identity matrix. */
    seq_t result[DIGIT_COUNT][DIGIT_COUNT];
    seq_t base[DIGIT_COUNT][DIGIT_COUNT];
    for (int i = 0; i < DIGIT_COUNT; ++i) {
        for (int j = 0; j < DIGIT_COUNT; ++j) {
            result[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
            base[i][j] = A[i][j];
        }
    }

    idx_t exp = n;
    while (exp > 0) {
        if (exp & (idx_t)1) {
            mat_mul(result, result, base);    /* result = result * base */
        }
        mat_mul(base, base, base);            /* base = base * base */
        exp >>= 1;
    }

    return mat_trace(result);
}
