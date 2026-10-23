/*
 * A125389  Number of base 26 circular n-digit numbers with adjacent digits
 *          differing by 5 or less.
 * https://oeis.org/A125389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) for n>=1, where A is a 26×26 0/1 matrix with
 *            A[i][j] = 1 if |i-j| <= 5 and 0 otherwise. a(0) = 1 (empty
 *            string). This counts base‑26 circular n‑digit numbers whose
 *            adjacent digits differ by at most 5
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

#define N 26  /* size of the adjacency matrix */

static seq_t adj[N][N];
static bool adj_inited = false;

/* initialize adjacency matrix once */
static void init_adj(void)
{
    if (adj_inited) return;
    adj_inited = true;
    for (idx_t i = 0; i < N; ++i) {
        for (idx_t j = 0; j < N; ++j) {
            idx_t diff = (i > j) ? (i - j) : (j - i);
            adj[i][j] = (diff <= 5) ? 1 : 0;
        }
    }
}

/* matrix multiplication: out = a * b  (both N×N) */
static void mat_mul(seq_t out[N][N], const seq_t a[N][N], const seq_t b[N][N])
{
    seq_t temp[N][N];
    for (idx_t i = 0; i < N; ++i)
        for (idx_t j = 0; j < N; ++j)
            temp[i][j] = 0;

    for (idx_t i = 0; i < N; ++i) {
        for (idx_t k = 0; k < N; ++k) {
            seq_t aik = a[i][k];
            if (aik == 0) continue;
            for (idx_t j = 0; j < N; ++j)
                temp[i][j] = temp[i][j] + aik * b[k][j];
        }
    }

    for (idx_t i = 0; i < N; ++i)
        for (idx_t j = 0; j < N; ++j)
            out[i][j] = temp[i][j];
}

/* identity matrix: I[i][i] = 1 else 0 */
static void identity(seq_t I[N][N])
{
    for (idx_t i = 0; i < N; ++i) {
        for (idx_t j = 0; j < N; ++j) I[i][j] = 0;
        I[i][i] = 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;   /* a(0) = 1 */

    init_adj();

    seq_t result[N][N];
    seq_t base[N][N];
    seq_t temp[N][N];

    identity(result);
    for (idx_t i = 0; i < N; ++i)
        for (idx_t j = 0; j < N; ++j)
            base[i][j] = adj[i][j];

    idx_t exp = n;
    while (exp) {
        if (exp & 1) {
            mat_mul(temp, result, base);
            for (idx_t i = 0; i < N; ++i)
                for (idx_t j = 0; j < N; ++j)
                    result[i][j] = temp[i][j];
        }
        exp >>= 1;
        if (exp) {
            mat_mul(temp, base, base);
            for (idx_t i = 0; i < N; ++i)
                for (idx_t j = 0; j < N; ++j)
                    base[i][j] = temp[i][j];
        }
    }

    seq_t trace = 0;
    for (idx_t i = 0; i < N; ++i)
        trace = trace + result[i][i];

    return trace;
}
