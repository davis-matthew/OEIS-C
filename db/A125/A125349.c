/*
 * A125349  Number of base 12 circular n-digit numbers with adjacent digits
 *          differing by 4 or less.
 * https://oeis.org/A125349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 1; a(n) = trace(A^n) for n > 0, where A is the 12×12
 *            matrix (A_{i,j} = 1 if |i - j| ≤ 4, otherwise 0). This counts
 *            base‑12 circular n‑digit numbers whose adjacent digits differ by
 *            4 or less
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>

#define DIGITS 12

/* Multiply two DIGITS×DIGITS matrices over seq_t: D = C × B. */
static void mat_mul(seq_t C[DIGITS][DIGITS], seq_t B[DIGITS][DIGITS],
                    seq_t D[DIGITS][DIGITS])
{
    seq_t tmp[DIGITS][DIGITS] = {0};
    for (int i = 0; i < DIGITS; ++i) {
        for (int j = 0; j < DIGITS; ++j) {
            seq_t sum = (seq_t)0;
            for (int k = 0; k < DIGITS; ++k) {
                sum += C[i][k] * B[k][j];
            }
            tmp[i][j] = sum;
        }
    }
    for (int i = 0; i < DIGITS; ++i)
        for (int j = 0; j < DIGITS; ++j)
            D[i][j] = tmp[i][j];
}

/* Compute E = base^exp using binary exponentiation. */
static void mat_pow(seq_t base[DIGITS][DIGITS],
                    idx_t exp,
                    seq_t result[DIGITS][DIGITS])
{
    seq_t identity[DIGITS][DIGITS] = {0};
    for (int i = 0; i < DIGITS; ++i)
        identity[i][i] = (seq_t)1;

    for (int i = 0; i < DIGITS; ++i)
        for (int j = 0; j < DIGITS; ++j)
            result[i][j] = identity[i][j];

    seq_t tmp[DIGITS][DIGITS];
    seq_t cur[DIGITS][DIGITS];
    for (int i = 0; i < DIGITS; ++i)
        for (int j = 0; j < DIGITS; ++j)
            cur[i][j] = base[i][j];

    while (exp > 0) {
        if (exp & 1) {
            mat_mul(result, cur, tmp);
            for (int i = 0; i < DIGITS; ++i)
                for (int j = 0; j < DIGITS; ++j)
                    result[i][j] = tmp[i][j];
        }
        mat_mul(cur, cur, tmp);
        for (int i = 0; i < DIGITS; ++i)
            for (int j = 0; j < DIGITS; ++j)
                cur[i][j] = tmp[i][j];
        exp >>= 1;
    }
}

/* Return the trace of a DIGITS×DIGITS matrix. */
static seq_t mat_trace(seq_t M[DIGITS][DIGITS])
{
    seq_t sum = (seq_t)0;
    for (int i = 0; i < DIGITS; ++i)
        sum += M[i][i];
    return sum;
}

/* Sequence function requested. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;                /* a(0) = 1 by convention */

    /* Build adjacency matrix A (difference ≤ 4). */
    seq_t A[DIGITS][DIGITS];
    for (int i = 0; i < DIGITS; ++i) {
        for (int j = 0; j < DIGITS; ++j) {
            int d = i > j ? i - j : j - i;
            A[i][j] = (d <= 4) ? (seq_t)1 : (seq_t)0;
        }
    }

    /* Compute A^n. */
    seq_t P[DIGITS][DIGITS];
    mat_pow(A, n, P);

    /* Return trace of A^n. */
    return mat_trace(P);
}
