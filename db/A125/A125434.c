/*
 * A125434  Number of base 22 circular n-digit numbers with adjacent digits
 *          differing by 7 or less.
 * https://oeis.org/A125434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n), for n ≥ 1, where A is the 22×22 adjacency *
 *            matrix with A[i][j] = 1 if |i‑j| ≤ 7 and 0 otherwise; a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>  /* for size_t, etc. */

static void mat_mul(const seq_t a[22][22], const seq_t b[22][22], seq_t res[22][22]) {
    for (idx_t i = 0; i < 22; ++i) {
        for (idx_t j = 0; j < 22; ++j) {
            seq_t sum = 0;
            for (idx_t k = 0; k < 22; ++k) {
                sum = sum + a[i][k] * b[k][j];
            }
            res[i][j] = sum;
        }
    }
}

static void matrix_pow(const seq_t base[22][22], idx_t exp, seq_t res[22][22]) {
    /* initialize result as identity matrix */
    for (idx_t i = 0; i < 22; ++i) {
        for (idx_t j = 0; j < 22; ++j) {
            res[i][j] = (i == j) ? 1 : 0;
        }
    }
    seq_t power[22][22];
    for (idx_t i = 0; i < 22; ++i)
        for (idx_t j = 0; j < 22; ++j)
            power[i][j] = base[i][j];

    seq_t tmp[22][22];
    while (exp > 0) {
        if (exp & 1) {
            mat_mul(res, power, tmp);
            for (idx_t i = 0; i < 22; ++i)
                for (idx_t j = 0; j < 22; ++j)
                    res[i][j] = tmp[i][j];
        }
        mat_mul(power, power, tmp);
        for (idx_t i = 0; i < 22; ++i)
            for (idx_t j = 0; j < 22; ++j)
                power[i][j] = tmp[i][j];
        exp >>= 1;
    }
}

seq_t a(idx_t n) {
    if (n == 0)
        return 1;
    static seq_t adj[22][22];
    static int initialized = 0;
    if (!initialized) {
        for (idx_t i = 0; i < 22; ++i) {
            for (idx_t j = 0; j < 22; ++j) {
                idx_t diff = (i > j) ? (i - j) : (j - i);
                adj[i][j] = (diff <= 7) ? 1 : 0;
            }
        }
        initialized = 1;
    }
    seq_t power[22][22];
    matrix_pow(adj, n, power);
    seq_t trace = 0;
    for (idx_t i = 0; i < 22; ++i)
        trace = trace + power[i][i];
    return trace;
}
