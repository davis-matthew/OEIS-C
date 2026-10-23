/*
 * A125465  Number of base 30 circular n-digit numbers with adjacent digits
 *          differing by 8 or less.
 * https://oeis.org/A125465
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 1 for n=0; for n>0 a(n) = trace(M^n) where M[i][j] = 1 if
 *            |i-j|≤8 else 0
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h> /* for size_t */

#define N_DIM 30


static void mat_mult(const seq_t A[N_DIM][N_DIM], const seq_t B[N_DIM][N_DIM], seq_t C[N_DIM][N_DIM]) {
    seq_t temp[N_DIM][N_DIM];
    for (int i = 0; i < N_DIM; ++i)
        for (int j = 0; j < N_DIM; ++j)
            temp[i][j] = (seq_t)0;
    for (int i = 0; i < N_DIM; ++i)
        for (int k = 0; k < N_DIM; ++k) {
            if (A[i][k] == (seq_t)0) continue;
            for (int j = 0; j < N_DIM; ++j) {
                if (B[k][j] == (seq_t)0) continue;
                temp[i][j] += A[i][k] * B[k][j];
            }
        }
    for (int i = 0; i < N_DIM; ++i)
        for (int j = 0; j < N_DIM; ++j)
            C[i][j] = temp[i][j];
}

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;

    static seq_t M[N_DIM][N_DIM];
    static int init = 0;
    if (!init) {
        init = 1;
        for (int i = 0; i < N_DIM; ++i) {
            for (int j = 0; j < N_DIM; ++j) {
                int diff = i > j ? i - j : j - i;
                M[i][j] = (diff <= 8) ? (seq_t)1 : (seq_t)0;
            }
        }
    }

    seq_t res[N_DIM][N_DIM];
    seq_t power[N_DIM][N_DIM];
    seq_t tmp[N_DIM][N_DIM];
    for (int i = 0; i < N_DIM; ++i)
        for (int j = 0; j < N_DIM; ++j) {
            res[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
            power[i][j] = M[i][j];
        }

    idx_t e = n;
    while (e) {
        if (e & (idx_t)1) {
            mat_mult(res, power, tmp);
            for (int i = 0; i < N_DIM; ++i)
                for (int j = 0; j < N_DIM; ++j)
                    res[i][j] = tmp[i][j];
        }
        e >>= 1;
        if (e) {
            mat_mult(power, power, tmp);
            for (int i = 0; i < N_DIM; ++i)
                for (int j = 0; j < N_DIM; ++j)
                    power[i][j] = tmp[i][j];
        }
    }

    seq_t sum = (seq_t)0;
    for (int i = 0; i < N_DIM; ++i)
        sum += res[i][i];
    return sum;
}
