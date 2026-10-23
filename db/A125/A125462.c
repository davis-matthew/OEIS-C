/*
 * A125462  Number of base 27 circular n-digit numbers with adjacent digits
 *          differing by 8 or less.
 * https://oeis.org/A125462
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(Adjacency^n), a(0) = 1 where Adjacency[i][j] = 1 if
 *            |i-j| <= 8, else 0. This counts circular n‑digit numbers in base
 *            27 whose adjacent digits differ by at most 8 (including the
 *            first and last digit)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define N 27   /* number of digits 0..26 */

static const seq_t base_adj[N][N] = {
    /* row 0 */
    {1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    /* row 1 */
    {1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    /* row 2 */
    {1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    /* row 3 */
    {1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    /* row 4 */
    {1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
    /* row 5 */
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
    /* row 6 */
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0},
    /* row 7 */
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0},
    /* row 8 */
    {1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
    /* row 9 */
    {0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
    /* row10 */
    {0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0},
    /* row11 */
    {0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0},
    /* row12 */
    {0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0},
    /* row13 */
    {0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0,0},
    /* row14 */
    {0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0,0},
    /* row15 */
    {0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0,0},
    /* row16 */
    {0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0,0},
    /* row17 */
    {0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0},
    /* row18 */
    {0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    /* row19 */
    {0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    /* row20 */
    {0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    /* row21 */
    {0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
    /* row22 */
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1},
    /* row23 */
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1},
    /* row24 */
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1},
    /* row25 */
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,1},
    /* row26 */
    {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1}
};

/* multiply two N×N matrices m1 and m2, store result in out */
static void mat_mul(const seq_t m1[N][N], const seq_t m2[N][N], seq_t out[N][N])
{
    seq_t tmp[N][N] = {0};
    for (idx_t i = 0; i < N; ++i) {
        for (idx_t k = 0; k < N; ++k) {
            if (m1[i][k] == 0) continue;
            for (idx_t j = 0; j < N; ++j) {
                tmp[i][j] = tmp[i][j] + m1[i][k] * m2[k][j];
            }
        }
    }
    for (idx_t i = 0; i < N; ++i)
        for (idx_t j = 0; j < N; ++j)
            out[i][j] = tmp[i][j];
}

/* exponentiate the adjacency matrix to power exp, return trace */
static seq_t trace_power(idx_t exp)
{
    /* identity matrix */
    seq_t res[N][N] = {0};
    for (idx_t i = 0; i < N; ++i) res[i][i] = 1;
    /* base matrix */
    seq_t base[N][N];
    for (idx_t i = 0; i < N; ++i)
        for (idx_t j = 0; j < N; ++j)
            base[i][j] = base_adj[i][j];

    while (exp) {
        if (exp & 1) {
            seq_t temp[N][N];
            mat_mul(res, base, temp);
            for (idx_t i = 0; i < N; ++i)
                for (idx_t j = 0; j < N; ++j)
                    res[i][j] = temp[i][j];
        }
        seq_t temp[N][N];
        mat_mul(base, base, temp);
        for (idx_t i = 0; i < N; ++i)
            for (idx_t j = 0; j < N; ++j)
                base[i][j] = temp[i][j];
        exp >>= 1;
    }

    seq_t tr = 0;
    for (idx_t i = 0; i < N; ++i)
        tr = tr + res[i][i];
    return tr;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    return trace_power(n);
}
