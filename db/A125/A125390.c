/*
 * A125390  Number of base 27 circular n-digit numbers with adjacent digits
 *          differing by 5 or less.
 * https://oeis.org/A125390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n), where A is the 27×27 adjacency matrix * with
 *            A[i][j] = 1 iff |i-j| ≤ 5, otherwise 0. * Special case: a(0) = 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

#define N 27

/* Multiply two N×N matrices of type seq_t.
 * The result is stored into res. */
static void mat_mult(seq_t a[N][N], seq_t b[N][N], seq_t res[N][N]) {
    seq_t tmp[N][N];
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < N; ++k) {
                sum += a[i][k] * b[k][j];
            }
            tmp[i][j] = sum;
        }
    }
    /* Copy back to res */
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            res[i][j] = tmp[i][j];
        }
    }
}

/* Exponentiate matrix base to exponent exp (exp ≥ 0).
 * The result is stored into res. */
static void mat_pow(seq_t base[N][N], idx_t exp, seq_t res[N][N]) {
    /* Initialize res as identity matrix */
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            res[i][j] = (i == j) ? 1 : 0;
        }
    }
    seq_t temp[N][N];
    seq_t cur[N][N];
    /* Copy base to cur */
    for (int i = 0; i < N; ++i) {
        for (int j = 0; j < N; ++j) {
            cur[i][j] = base[i][j];
        }
    }
    while (exp > 0) {
        if (exp & 1) {
            mat_mult(res, cur, temp);
            for (int i = 0; i < N; ++i)
                for (int j = 0; j < N; ++j)
                    res[i][j] = temp[i][j];
        }
        /* cur = cur * cur */
        mat_mult(cur, cur, temp);
        for (int i = 0; i < N; ++i)
            for (int j = 0; j < N; ++j)
                cur[i][j] = temp[i][j];
        exp >>= 1;
    }
}

/* The main function computing a(n). */
seq_t a(idx_t n) {
    if (n == 0) return 1;  /* by definition of the sequence */

    /* Static adjacency matrix to avoid recomputation */
    static seq_t adj[N][N];
    static int initialized = 0;
    if (!initialized) {
        for (int i = 0; i < N; ++i) {
            for (int j = 0; j < N; ++j) {
                int diff = i - j;
                if (diff < 0) diff = -diff;
                adj[i][j] = (diff <= 5) ? 1 : 0;
            }
        }
        initialized = 1;
    }

    seq_t power[N][N];
    mat_pow(adj, n, power);

    /* Compute trace of power */
    seq_t result = 0;
    for (int i = 0; i < N; ++i) {
        result += power[i][i];
    }
    return result;
}
