/*
 * A125326  Number of base 16 circular n-digit numbers with adjacent digits
 *          differing by 3 or less.
 * https://oeis.org/A125326
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 1; for n >= 1, a(n) = trace(A^n) where A[i][j] = 1 if
 *            |i-j| <= 3 else 0
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static void matrix_mul(const seq_t a[16][16], const seq_t b[16][16], seq_t res[16][16]) {
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < 16; ++k) {
                sum += a[i][k] * b[k][j];
            }
            res[i][j] = sum;
        }
    }
}

seq_t a(idx_t n) {
    if (n == 0) {
        return (seq_t)1;
    }

    /* Build the adjacency matrix once */
    static seq_t base[16][16];
    static bool built = false;
    if (!built) {
        for (int i = 0; i < 16; ++i) {
            for (int j = 0; j < 16; ++j) {
                int diff = (i > j) ? (i - j) : (j - i);
                base[i][j] = (diff <= 3) ? (seq_t)1 : (seq_t)0;
            }
        }
        built = true;
    }

    seq_t result[16][16];
    seq_t power[16][16];
    /* Initialize result to identity, power to base */
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            result[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
            power[i][j] = base[i][j];
        }
    }

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            seq_t temp[16][16];
            matrix_mul(result, power, temp);
            for (int i = 0; i < 16; ++i)
                for (int j = 0; j < 16; ++j)
                    result[i][j] = temp[i][j];
        }
        seq_t temp[16][16];
        matrix_mul(power, power, temp);
        for (int i = 0; i < 16; ++i)
            for (int j = 0; j < 16; ++j)
                power[i][j] = temp[i][j];
        exp >>= 1;
    }

    seq_t trace = 0;
    for (int i = 0; i < 16; ++i) {
        trace += result[i][i];
    }
    return trace;
}
