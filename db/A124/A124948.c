/*
 * A124948  Number of base 20 circular n-digit numbers with adjacent digits
 *          differing by 2 or less.
 * https://oeis.org/A124948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = trace(A^n) where A[i][j] = 1 if |i-j| ≤ 2 else 0 (with
 *            a(0)=1, a(1)=20)
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

#define B 20      /* base 20 */

static void mat_identity(seq_t m[B][B]) {
    for (idx_t i = 0; i < B; ++i)
        for (idx_t j = 0; j < B; ++j)
            m[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
}

static void mat_copy(seq_t dst[B][B], const seq_t src[B][B]) {
    for (idx_t i = 0; i < B; ++i)
        for (idx_t j = 0; j < B; ++j)
            dst[i][j] = src[i][j];
}

static void mat_mul(seq_t res[B][B], const seq_t a[B][B], const seq_t b[B][B]) {
    seq_t tmp[B][B];
    for (idx_t i = 0; i < B; ++i) {
        for (idx_t j = 0; j < B; ++j) {
            seq_t sum = 0;
            for (idx_t k = 0; k < B; ++k)
                sum += a[i][k] * b[k][j];
            tmp[i][j] = sum;
        }
    }
    mat_copy(res, tmp);
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)20;

    /* Build adjacency matrix A */
    seq_t A[B][B];
    for (idx_t i = 0; i < B; ++i) {
        for (idx_t j = 0; j < B; ++j) {
            idx_t diff = (i > j) ? (i - j) : (j - i);
            A[i][j] = (diff <= 2) ? (seq_t)1 : (seq_t)0;
        }
    }

    /* result = identity, base = A */
    seq_t result[B][B];
    seq_t base[B][B];
    mat_identity(result);
    mat_copy(base, A);

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            seq_t tmp[B][B];
            mat_mul(tmp, result, base);
            mat_copy(result, tmp);
        }
        /* base = base * base */
        seq_t tmp[B][B];
        mat_mul(tmp, base, base);
        mat_copy(base, tmp);

        exp >>= 1;
    }

    /* trace of A^n */
    seq_t trace = 0;
    for (idx_t i = 0; i < B; ++i)
        trace += result[i][i];
    return trace;
}
