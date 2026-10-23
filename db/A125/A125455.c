/*
 * A125455  Number of base 20 circular n-digit numbers with adjacent digits
 *          differing by 8 or less.
 * https://oeis.org/A125455
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n), where A[i][j] = 1 if |i-j| ≤ 8, otherwise 0
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define DIGITS 20

/* Multiply two DIGITS×DIGITS matrices: res = a · b.  */
static void mul_matrix(const seq_t a[DIGITS][DIGITS],
                       const seq_t b[DIGITS][DIGITS],
                       seq_t res[DIGITS][DIGITS])
{
    for (int i = 0; i < DIGITS; ++i) {
        for (int j = 0; j < DIGITS; ++j) {
            seq_t sum = (seq_t)0;
            for (int k = 0; k < DIGITS; ++k) {
                sum += a[i][k] * b[k][j];
            }
            res[i][j] = sum;
        }
    }
}

/* Copy matrix src into dst. */
static void copy_matrix(const seq_t src[DIGITS][DIGITS],
                        seq_t dst[DIGITS][DIGITS])
{
    for (int i = 0; i < DIGITS; ++i)
        for (int j = 0; j < DIGITS; ++j)
            dst[i][j] = src[i][j];
}

seq_t a(idx_t n)
{
    /* The empty number is counted once. */
    if (n == 0)
        return (seq_t)1;

    /* Adjacency matrix A (only built once). */
    static seq_t A[DIGITS][DIGITS];
    static int initialized = 0;
    if (!initialized) {
        for (int i = 0; i < DIGITS; ++i) {
            for (int j = 0; j < DIGITS; ++j) {
                int diff = (i > j) ? (i - j) : (j - i);
                A[i][j] = (diff <= 8) ? (seq_t)1 : (seq_t)0;
            }
        }
        initialized = 1;
    }

    /* Result=multiplicative identity, Power=A. */
    seq_t Result[DIGITS][DIGITS];
    seq_t Power[DIGITS][DIGITS];
    for (int i = 0; i < DIGITS; ++i) {
        for (int j = 0; j < DIGITS; ++j) {
            Result[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
            Power[i][j]  = A[i][j];
        }
    }

    idx_t exp = n;
    seq_t Temp[DIGITS][DIGITS];

    /* Binary exponentiation of the matrix. */
    while (exp > 0) {
        if (exp & 1ULL) {
            mul_matrix(Result, Power, Temp);
            copy_matrix(Temp, Result);
        }
        exp >>= 1;
        if (!exp) break;
        mul_matrix(Power, Power, Temp);
        copy_matrix(Temp, Power);
    }

    /* Trace of Result gives the number of circular n‑digit numbers. */
    seq_t trace = (seq_t)0;
    for (int i = 0; i < DIGITS; ++i)
        trace += Result[i][i];

    return trace;
}
