/*
 * A125431  Number of base 19 circular n-digit numbers with adjacent digits
 *          differing by 7 or less.
 * https://oeis.org/A125431
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(AdjacencyMatrix^n) for n>=1 * a(0) = 1 * * Where
 *            the graph has vertices 0..18 and an edge * between i and j iff
 *            |i-j| <= 7. The adjacency * matrix is 19×19. The trace of its
 *            n-th power * counts cyclic n‑digit base‑19 numbers whose *
 *            consecutive digits differ by at most 7
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

#define N 19          /* number of digits (vertices)      */
#define TH 7          /* maximum allowed difference      */

static void mul_matrix(const seq_t a[N][N], const seq_t b[N][N], seq_t res[N][N])
{
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < N; ++k) {
                sum += a[i][k] * b[k][j];
            }
            res[i][j] = sum;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) {
        return 1;
    }

    /* Base adjacency matrix */
    static seq_t base[N][N];
    static bool init = false;
    if (!init) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                base[i][j] = (( (i >= j) ? (i - j) : (j - i) ) <= TH) ? 1 : 0;
            }
        }
        init = true;
    }

    /* Res = identity matrix */
    seq_t res[N][N] = {0};
    for (int i = 0; i < N; ++i) {
        res[i][i] = 1;
    }

    /* Power exponentiation */
    seq_t a_pow[N][N];
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            a_pow[i][j] = base[i][j];
        }
    }

    idx_t exp = n;
    seq_t temp[N][N];

    while (exp) {
        if (exp & 1) {
            mul_matrix(res, a_pow, temp);
            /* copy temp into res */
            for (int i = 0; i < N; ++i)
                for (int j = 0; j < N; ++j)
                    res[i][j] = temp[i][j];
        }
        /* a_pow = a_pow * a_pow */
        mul_matrix(a_pow, a_pow, temp);
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                a_pow[i][j] = temp[i][j];

        exp >>= 1;
    }

    /* Compute trace */
    seq_t ans = 0;
    for (int i = 0; i < N; ++i) {
        ans += res[i][i];
    }
    return ans;
}
