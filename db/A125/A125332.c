/*
 * A125332  Number of base 22 circular n-digit numbers with adjacent digits
 *          differing by 3 or less.
 * https://oeis.org/A125332
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) for n>=1 (base 22 digist bipartite adjacency)
 *            where A[i][j] = 1 if |i-j| <= 3, else 0. a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define N 22

static void mat_mul(const seq_t A[N][N], const seq_t B[N][N], seq_t C[N][N]) {
    /* Compute C = A * B  (matrix multiplication over seq_t) */
    seq_t temp[N][N];
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < N; ++j)
            temp[i][j] = (seq_t)0;

    for (size_t i = 0; i < N; ++i) {
        for (size_t k = 0; k < N; ++k) {
            seq_t aik = A[i][k];
            if (aik == (seq_t)0) continue;
            for (size_t j = 0; j < N; ++j) {
                temp[i][j] += aik * B[k][j];
            }
        }
    }

    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < N; ++j)
            C[i][j] = temp[i][j];
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    /* Base adjacency matrix for base-22 digits with |i-j| <= 3 */
    static seq_t base[N][N];
    static int base_init = 0;
    if (!base_init) {
        for (size_t i = 0; i < N; ++i) {
            for (size_t j = 0; j < N; ++j) {
                size_t diff;
                if (i > j) diff = i - j;
                else diff = j - i;
                base[i][j] = (diff <= 3) ? (seq_t)1 : (seq_t)0;
            }
        }
        base_init = 1;
    }

    /* identity matrix */
    seq_t res[N][N];
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < N; ++j)
            res[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;

    /* current power of base matrix */
    seq_t cur[N][N];
    for (size_t i = 0; i < N; ++i)
        for (size_t j = 0; j < N; ++j)
            cur[i][j] = base[i][j];

    idx_t exp = n;
    seq_t prod[N][N];
    while (exp > 0) {
        if (exp & 1) {
            mat_mul(res, cur, prod);
            for (size_t i = 0; i < N; ++i)
                for (size_t j = 0; j < N; ++j)
                    res[i][j] = prod[i][j];
        }
        exp >>= 1;
        if (exp) {
            mat_mul(cur, cur, prod);
            for (size_t i = 0; i < N; ++i)
                for (size_t j = 0; j < N; ++j)
                    cur[i][j] = prod[i][j];
        }
    }

    /* trace of A^n */
    seq_t ans = (seq_t)0;
    for (size_t i = 0; i < N; ++i)
        ans += res[i][i];
    return ans;
}
