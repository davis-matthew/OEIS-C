/*
 * A125325  Number of base 15 circular n-digit numbers with adjacent digits
 *          differing by 3 or less.
 * https://oeis.org/A125325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of base‑15 circular n‑digit numbers with adjacent
 *            digits differing by 3 or less. For n = 0 a(0) = 1, otherwise
 *            a(n) = trace(A^n), where A is the 15×15 adjacency matrix with
 *            A[i][j] = 1 if |i–j| ≤ 3 else 0
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static void matrix_mult(const seq_t a[15][15], const seq_t b[15][15], seq_t c[15][15]) {
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < 15; ++k) {
                sum += a[i][k] * b[k][j];
            }
            c[i][j] = sum;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    /* Build the adjacency matrix once. */
    static seq_t base[15][15];
    static int initialized = 0;
    if (!initialized) {
        for (int i = 0; i < 15; ++i) {
            for (int j = 0; j < 15; ++j) {
                int diff = i - j;
                if (diff < 0) diff = -diff;
                base[i][j] = (diff <= 3) ? (seq_t)1 : (seq_t)0;
            }
        }
        initialized = 1;
    }

    /* Matrix exponentiation: result = I, power = base. */
    seq_t result[15][15];
    seq_t power[15][15];
    seq_t temp[15][15];

    /* identity matrix */
    for (int i = 0; i < 15; ++i) {
        for (int j = 0; j < 15; ++j) {
            result[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
            power[i][j] = base[i][j];
        }
    }

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            matrix_mult(result, power, temp);
            for (int i = 0; i < 15; ++i)
                for (int j = 0; j < 15; ++j)
                    result[i][j] = temp[i][j];
        }
        matrix_mult(power, power, temp);
        for (int i = 0; i < 15; ++i)
            for (int j = 0; j < 15; ++j)
                power[i][j] = temp[i][j];
        exp >>= 1;
    }

    /* Sum the diagonal to get the trace. */
    seq_t trace = 0;
    for (int i = 0; i < 15; ++i) {
        trace += result[i][i];
    }
    return trace;
}
