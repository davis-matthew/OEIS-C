/*
 * A124947  Number of base 19 circular n-digit numbers with adjacent digits
 *          differing by 2 or less.
 * https://oeis.org/A124947
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = trace(A^n) for the 19×19 adjacency matrix A where
 * A[i][j] = 1 if |i-j| ≤ 2 (including self‑loops), else 0.
 * The function uses binary exponentiation and works for any
 * seq_t and idx_t provided by the build. */

#include <stdint.h>
#include <stdbool.h>

typedef struct {
    seq_t m[19][19];
} Mat;

/* multiply two 19×19 matrices over seq_t */
static Mat mat_mul(const Mat *x, const Mat *y) {
    Mat r;
    for (uint8_t i = 0; i < 19; ++i) {
        for (uint8_t j = 0; j < 19; ++j) {
            seq_t sum = 0;
            for (uint8_t k = 0; k < 19; ++k) {
                sum += x->m[i][k] * y->m[k][j];
            }
            r.m[i][j] = sum;
        }
    }
    return r;
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    /* adjacency matrix for base 19, computed once */
    static Mat adjacency;
    static bool init = false;
    if (!init) {
        init = true;
        for (uint8_t i = 0; i < 19; ++i) {
            for (uint8_t j = 0; j < 19; ++j) {
                adjacency.m[i][j] = ((i >= j ? i - j : j - i) <= 2) ? (seq_t)1 : (seq_t)0;
            }
        }
    }

    /* identity matrix */
    Mat result;
    for (uint8_t i = 0; i < 19; ++i)
        for (uint8_t j = 0; j < 19; ++j)
            result.m[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;

    /* base = adjacency */
    Mat base;
    for (uint8_t i = 0; i < 19; ++i)
        for (uint8_t j = 0; j < 19; ++j)
            base.m[i][j] = adjacency.m[i][j];

    idx_t exp = n;
    Mat temp;
    while (exp > 0) {
        if (exp & 1) {
            /* result = result * base */
            result = mat_mul(&result, &base);
        }
        /* base = base * base */
        temp = mat_mul(&base, &base);
        base = temp;
        exp >>= 1;
    }

    /* trace of A^n */
    seq_t trace = 0;
    for (uint8_t i = 0; i < 19; ++i) trace += result.m[i][i];
    return trace;
}
