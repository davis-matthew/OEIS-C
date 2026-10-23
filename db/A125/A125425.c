/*
 * A125425  Number of base 13 circular n-digit numbers with adjacent digits
 *          differing by 7 or less.
 * https://oeis.org/A125425
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(Aⁿ) for n>0, a(0)=1, where A[i][j] = 1 if |i-j| ≤ 7
 *            else 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Helper: multiply two 13×13 matrices over seq_t */
static void mat_mul(const seq_t a[13][13], const seq_t b[13][13], seq_t res[13][13])
{
    for (int i = 0; i < 13; ++i) {
        for (int j = 0; j < 13; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < 13; ++k) {
                if (b[k][j] != 0) {
                    sum += a[i][k] * b[k][j];
                }
            }
            res[i][j] = sum;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }

    const int N = 13;

    /* Base adjacency matrix A */
    seq_t base[N][N];
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            int diff = i >= j ? i - j : j - i;
            base[i][j] = (seq_t)((diff <= 7) ? 1 : 0);
        }
    }

    /* Identity matrix (initial result) */
    seq_t res[N][N];
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            res[i][j] = (seq_t)((i == j) ? 1 : 0);
        }
    }

    idx_t exp = n;
    seq_t tmp[N][N];

    /* Binary exponentiation: res = baseⁿ */
    while (exp) {
        if (exp & 1) {
            mat_mul(res, base, tmp);
            for (int i = 0; i < N; ++i)
                for (int j = 0; j < N; ++j)
                    res[i][j] = tmp[i][j];
        }
        mat_mul(base, base, tmp);
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                base[i][j] = tmp[i][j];
        exp >>= 1;
    }

    /* Sum diagonal entries: trace(Aⁿ) */
    seq_t trace = 0;
    for (int i = 0; i < N; ++i) {
        trace += res[i][i];
    }
    return trace;
}
