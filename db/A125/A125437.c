/*
 * A125437  Number of base 25 circular n-digit numbers with adjacent digits
 *          differing by 7 or less.
 * https://oeis.org/A125437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n), where A is the 25×25 adjacency matrix with
 *            A[i][j] = 1 if |i-j| ≤ 7 else 0
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   matrix_identity
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/matrix_identity.h"

/* Helper: multiply two 25×25 matrices. */
static void matrix_mul(seq_t out[25][25],
                       const seq_t a[25][25],
                       const seq_t b[25][25])
{
    for (int i = 0; i < 25; ++i) {
        for (int j = 0; j < 25; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < 25; ++k) {
                if (a[i][k] && b[k][j]) {
                    sum += a[i][k] * b[k][j];
                }
            }
            out[i][j] = sum;
        }
    }
}

/* Helper: exponentiate matrix by n using binary exponentiation. */
static void matrix_pow(seq_t result[25][25],
                       const seq_t base[25][25],
                       idx_t n)
{
    seq_t res[25][25];
    seq_t cur[25][25];
    seq_t tmp[25][25];

    matrix_identity(res);
    for (int i = 0; i < 25; ++i)
        for (int j = 0; j < 25; ++j)
            cur[i][j] = base[i][j];

    while (n > 0) {
        if (n & 1) {
            matrix_mul(tmp, res, cur);
            for (int i = 0; i < 25; ++i)
                for (int j = 0; j < 25; ++j)
                    res[i][j] = tmp[i][j];
        }
        matrix_mul(tmp, cur, cur);
        for (int i = 0; i < 25; ++i)
            for (int j = 0; j < 25; ++j)
                cur[i][j] = tmp[i][j];
        n >>= 1;
    }

    for (int i = 0; i < 25; ++i)
        for (int j = 0; j < 25; ++j)
            result[i][j] = res[i][j];
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 1;

    /* Build adjacency matrix A. */
    seq_t A[25][25];
    for (int i = 0; i < 25; ++i) {
        for (int j = 0; j < 25; ++j) {
            idx_t diff = (i > j) ? (i - j) : (j - i);
            A[i][j] = (diff <= 7) ? 1 : 0;
        }
    }

    /* Compute A^n. */
    seq_t Mn[25][25];
    matrix_pow(Mn, A, n);

    /* Sum diagonal entries to get trace. */
    seq_t trace = 0;
    for (int i = 0; i < 25; ++i)
        trace += Mn[i][i];

    return trace;
}
