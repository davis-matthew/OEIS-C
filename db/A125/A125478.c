/*
 * A125478  Number of base 21 circular n-digit numbers with adjacent digits
 *          differing by 9 or less.
 * https://oeis.org/A125478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) where A is the 21×21 adjacency matrix of the
 *            graph on digits 0…20 with edges between i and j iff |i‑j| ≤ 9.
 *            a(0) is defined to be 1. The function returns the number of
 *            base‑21 circular n‑digit numbers whose adjacent digits differ by
 *            at most 9
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* multiply two 21×21 matrices of type seq_t */
static void mul_matrix(const seq_t a[21][21], const seq_t b[21][21], seq_t c[21][21]) {
    for (int i = 0; i < 21; ++i) {
        for (int j = 0; j < 21; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < 21; ++k) {
                sum += a[i][k] * b[k][j];
            }
            c[i][j] = sum;
        }
    }
}

/* sequence entry */
seq_t a(idx_t n) {
    /* Special case: a(0) = 1 by definition */
    if (n == 0) return (seq_t)1;

    /* Build adjacency matrix once */
    static seq_t adj[21][21];
    static bool init = false;
    if (!init) {
        init = true;
        for (int i = 0; i < 21; ++i) {
            for (int j = 0; j < 21; ++j) {
                int d = (i > j) ? (i - j) : (j - i);
                adj[i][j] = (d <= 9) ? (seq_t)1 : (seq_t)0;
            }
        }
    }

    /* Fast exponentiation: result = I, power = adj */
    seq_t result[21][21] = {0};
    seq_t power[21][21];
    seq_t temp[21][21];
    for (int i = 0; i < 21; ++i)
        for (int j = 0; j < 21; ++j)
            result[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;

    for (int i = 0; i < 21; ++i)
        for (int j = 0; j < 21; ++j)
            power[i][j] = adj[i][j];

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            mul_matrix(result, power, temp);
            for (int i = 0; i < 21; ++i)
                for (int j = 0; j < 21; ++j)
                    result[i][j] = temp[i][j];
        }
        mul_matrix(power, power, temp);
        for (int i = 0; i < 21; ++i)
            for (int j = 0; j < 21; ++j)
                power[i][j] = temp[i][j];
        exp >>= 1;
    }

    /* Compute trace of result */
    seq_t trace = 0;
    for (int i = 0; i < 21; ++i)
        trace += result[i][i];

    return trace;
}
