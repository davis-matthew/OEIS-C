/*
 * A125337  Number of base 27 circular n-digit numbers with adjacent digits
 *          differing by 3 or less.
 * https://oeis.org/A125337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula:  a(n) = 1       , n = 0
 *           a(n) = trace(A^n), n ≥ 1
 * where A[i][j] = 1 if |i-j| ≤ 3 (0 ≤ i,j ≤ 26), otherwise 0.
 * This counts circular base‑27 n‑digit numbers with adjacent digits
 * differing by at most 3.
 */

#include <stdbool.h>
#include <stdint.h>

#define N 27  /* number of digits in base 27 */

static bool _adj_init = false;
static seq_t _adj[N][N];

/* Initialize the adjacency matrix once */
static void _init_adj(void)
{
    if (_adj_init) return;
    for (idx_t i = 0; i < N; ++i) {
        for (idx_t j = 0; j < N; ++j) {
            seq_t one = (seq_t)1;
            seq_t zero = (seq_t)0;
            if (i == j) {
                _adj[i][j] = one;
            } else {
                idx_t d = (i > j) ? (i - j) : (j - i);
                _adj[i][j] = (d <= 3) ? one : zero;
            }
        }
    }
    _adj_init = true;
}

/* Matrix multiplication: out = A * B  (all N × N) */
static void _mat_mul(seq_t out[N][N], const seq_t A[N][N], const seq_t B[N][N])
{
    for (idx_t i = 0; i < N; ++i) {
        for (idx_t j = 0; j < N; ++j) {
            seq_t sum = (seq_t)0;
            for (idx_t k = 0; k < N; ++k) {
                sum += A[i][k] * B[k][j];
            }
            out[i][j] = sum;
        }
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* empty number */

    _init_adj();

    /* Identity matrix R = I */
    seq_t R[N][N];
    for (idx_t i = 0; i < N; ++i) {
        for (idx_t j = 0; j < N; ++j) {
            R[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
        }
    }

    seq_t M[N][N];
    for (idx_t i = 0; i < N; ++i)
        for (idx_t j = 0; j < N; ++j)
            M[i][j] = _adj[i][j];

    idx_t exp = n;
    seq_t tmp[N][N];

    while (exp > 0) {
        if (exp & 1) {
            _mat_mul(tmp, R, M);
            for (idx_t i = 0; i < N; ++i)
                for (idx_t j = 0; j < N; ++j)
                    R[i][j] = tmp[i][j];
        }
        _mat_mul(tmp, M, M);
        for (idx_t i = 0; i < N; ++i)
            for (idx_t j = 0; j < N; ++j)
                M[i][j] = tmp[i][j];
        exp >>= 1;
    }

    /* Compute trace of R */
    seq_t trace = (seq_t)0;
    for (idx_t i = 0; i < N; ++i)
        trace += R[i][i];

    return trace;
}
