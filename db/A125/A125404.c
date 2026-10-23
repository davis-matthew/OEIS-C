/*
 * A125404  Number of base 16 circular n-digit numbers with adjacent digits
 *          differing by 6 or less.
 * https://oeis.org/A125404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n), A[i][j] = 1 if |i-j| ≤ 6, else 0. a(0) = 1
 *            (empty string)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Matrix multiplication: C = A*B  (all matrices are 16×16) */
static void mat_mul(const seq_t A[][16], const seq_t B[][16], seq_t C[][16]) {
    for (int i = 0; i < 16; ++i) {
        for (int j = 0; j < 16; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < 16; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

/* Copy matrix src into dst  (16×16) */
static void mat_copy(seq_t dst[][16], const seq_t src[][16]) {
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            dst[i][j] = src[i][j];
}

seq_t a(idx_t n) {
    if (n == 0) return 1;            /* a(0) = 1 by definition */

    /* ---  adjacency matrix A  --- */
    static seq_t A[16][16];
    static bool A_inited = false;
    if (!A_inited) {
        for (int i = 0; i < 16; ++i) {
            for (int j = 0; j < 16; ++j) {
                int diff = i >= j ? i - j : j - i;
                A[i][j] = (diff <= 6) ? 1 : 0;
            }
        }
        A_inited = true;
    }

    /* --- matrix exponentiation: compute A^n  --- */
    seq_t res[16][16];
    seq_t power[16][16];
    seq_t tmp[16][16];

    /* identity matrix for res */
    for (int i = 0; i < 16; ++i)
        for (int j = 0; j < 16; ++j)
            res[i][j] = (i == j) ? 1 : 0;

    mat_copy(power, A);

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            mat_mul(res, power, tmp);
            mat_copy(res, tmp);
        }
        mat_mul(power, power, tmp);
        mat_copy(power, tmp);
        exp >>= 1;
    }

    /* --- trace of A^n --- */
    seq_t sum = 0;
    for (int i = 0; i < 16; ++i) sum += res[i][i];
    return sum;
}
