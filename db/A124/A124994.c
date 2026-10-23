/*
 * A124994  Number of base 31 circular n-digit numbers with adjacent digits
 *          differing by 2 or less.
 * https://oeis.org/A124994
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = trace(A^n) where A[i][j] = 1 if |i-j| ≤ 2 else 0. The
 *            matrix A is 31×31
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The adjacency matrix is fixed, so we can pre‑initialize it once. */
#include <stdbool.h>

#define BASE 31

/* Multiply two 31×31 matrices of type seq_t.  The result is stored in C. */
static void mat_mul(const seq_t A[BASE][BASE], const seq_t B[BASE][BASE], seq_t C[BASE][BASE])
{
    /* We use typed indices for clarity; the loop bounds are tiny. */
    idx_t i, j, k;
    for (i = 0; i < BASE; ++i) {
        for (j = 0; j < BASE; ++j) {
            seq_t sum = 0;
            for (k = 0; k < BASE; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

/* Return the a(n) value for the sequence. */
seq_t a(idx_t n)
{
    /* Trivial case: a(0) = 1. */
    if (n == 0) {
        return (seq_t)1;
    }

    /* Base adjacency matrix A.  Static prevents recomputation. */
    static seq_t base[BASE][BASE];
    static bool init = false;
    if (!init) {
        idx_t i, j;
        for (i = 0; i < BASE; ++i) {
            for (j = 0; j < BASE; ++j) {
                /* Use idx_t for the subtraction to avoid underflow. */
                idx_t diff = (i > j) ? (i - j) : (j - i);
                base[i][j] = (diff <= 2) ? 1 : 0;
            }
        }
        init = true;
    }

    /* Result matrix will hold the current power of A. */
    seq_t result[BASE][BASE];
    seq_t power[BASE][BASE];
    seq_t temp[BASE][BASE];
    idx_t i, k;

    /* Identity matrix in result (for exponentiation by squaring). */
    for (i = 0; i < BASE; ++i) {
        for (k = 0; k < BASE; ++k) {
            result[i][k] = (i == k) ? 1 : 0;
        }
    }

    /* Copy base to power. */
    for (i = 0; i < BASE; ++i) {
        for (k = 0; k < BASE; ++k) {
            power[i][k] = base[i][k];
        }
    }

    /* Fast exponentiation using the binary representation of n. */
    while (n > 0) {
        if (n & 1ULL) {
            mat_mul(result, power, temp);
            for (i = 0; i < BASE; ++i) {
                for (k = 0; k < BASE; ++k) {
                    result[i][k] = temp[i][k];
                }
            }
        }
        mat_mul(power, power, temp);
        for (i = 0; i < BASE; ++i) {
            for (k = 0; k < BASE; ++k) {
                power[i][k] = temp[i][k];
            }
        }
        n >>= 1;
    }

    /* Sum the diagonal: trace(A^n). */
    seq_t trace = 0;
    for (i = 0; i < BASE; ++i) {
        trace += result[i][i];
    }
    return trace;
}
