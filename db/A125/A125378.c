/*
 * A125378  Number of base 15 circular n-digit numbers with adjacent digits
 *          differing by 5 or less.
 * https://oeis.org/A125378
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(M^n) where M[i][j] = 1 if |i - j| <= 5 else 0
 *            (base‑15 adjacency)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define BASE 15

static void mat_mul(seq_t out[BASE][BASE],
                    const seq_t a[BASE][BASE],
                    const seq_t b[BASE][BASE]) {
    for (idx_t i = 0; i < BASE; ++i) {
        for (idx_t j = 0; j < BASE; ++j) {
            seq_t sum = 0;
            for (idx_t k = 0; k < BASE; ++k) {
                sum += a[i][k] * b[k][j];
            }
            out[i][j] = sum;
        }
    }
}

static void mat_copy(seq_t dst[BASE][BASE], const seq_t src[BASE][BASE]) {
    for (idx_t i = 0; i < BASE; ++i)
        for (idx_t j = 0; j < BASE; ++j)
            dst[i][j] = src[i][j];
}

seq_t a(idx_t n) {
    /* a(0) = 1 by definition */
    if (n == 0)
        return (seq_t)1;

    static bool initialized = false;
    static seq_t base[BASE][BASE];
    if (!initialized) {
        for (idx_t i = 0; i < BASE; ++i) {
            for (idx_t j = 0; j < BASE; ++j) {
                idx_t diff = (i > j) ? i - j : j - i;
                base[i][j] = (diff <= 5) ? (seq_t)1 : (seq_t)0;
            }
        }
        initialized = true;
    }

    seq_t result[BASE][BASE];
    seq_t cur[BASE][BASE];
    seq_t tmp[BASE][BASE];

    /* identity matrix */
    for (idx_t i = 0; i < BASE; ++i)
        for (idx_t j = 0; j < BASE; ++j)
            result[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;

    mat_copy(cur, base);

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            mat_mul(tmp, result, cur);
            mat_copy(result, tmp);
        }
        mat_mul(tmp, cur, cur);
        mat_copy(cur, tmp);
        exp >>= 1;
    }

    seq_t trace = 0;
    for (idx_t i = 0; i < BASE; ++i)
        trace += result[i][i];

    return trace;
}
