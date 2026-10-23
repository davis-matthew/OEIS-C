/*
 * A125413  Number of base 25 circular n-digit numbers with adjacent digits
 *          differing by 6 or less.
 * https://oeis.org/A125413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(0) = 1; for n>=1, a(n) = trace(A^n) where A[i][j] = 1 if |i-j|
 *            ≤ 6 else 0. * A is a 25×25 adjacency matrix for the directed
 *            graph of digits 0–24 with edges * between digits whose
 *            difference is at most 6. trace(A^n) counts all n‑digit *
 *            circular sequences satisfying the adjacency condition
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

#define N 25  /* number of digits in base 25 */

/* adjacency matrix initialised once */
static seq_t adj[N][N];
static bool adj_inited = false;
static void init_adj(void)
{
    if (adj_inited)
        return;
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            adj[i][j] = ((i > j ? i - j : j - i) <= 6) ? (seq_t)1 : (seq_t)0;
        }
    }
    adj_inited = true;
}

/* matrix multiplication: c = a * b */
static void mat_mult(const seq_t a[N][N], const seq_t b[N][N], seq_t c[N][N])
{
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < N; ++k) {
                sum += a[i][k] * b[k][j];
            }
            c[i][j] = sum;
        }
    }
}

/* public entry point */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    init_adj();

    seq_t res[N][N];
    seq_t base[N][N];
    seq_t tmp[N][N];

    /* initialise result as identity matrix, base as adjacency matrix copy */
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            res[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
            base[i][j] = adj[i][j];
        }
    }

    idx_t e = n;
    while (e > 0) {
        if (e & 1) {
            mat_mult(res, base, tmp);
            for (int i = 0; i < N; ++i)
                for (int j = 0; j < N; ++j)
                    res[i][j] = tmp[i][j];
        }
        mat_mult(base, base, tmp);
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                base[i][j] = tmp[i][j];
        e >>= 1;
    }

    /* compute trace of res */
    seq_t sum = 0;
    for (int i = 0; i < N; ++i)
        sum += res[i][i];
    return sum;
}
