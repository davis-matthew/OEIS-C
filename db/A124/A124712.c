/*
 * A124712  Number of base 19 circular n-digit numbers with adjacent digits
 *          differing by 1 or less.
 * https://oeis.org/A124712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) where A is the 19×19 adjacency matrix of the
 *            graph on vertices {1,…,19} with an edge (i,j) iff |i−j|≤1. The
 *            trace of the n‑th power counts the smooth cyclic words of length
 *            n over the alphabet {1,…,19}
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Number of vertices in the adjacency graph. */
#define K 19

/* The base adjacency matrix (static to avoid repeated construction). */
static seq_t base[K][K];
static bool base_initialized = false;

/* Matrix multiplication: res = a * b  (all K×K). */
static void mat_mult(const seq_t a[K][K], const seq_t b[K][K], seq_t res[K][K])
{
    for (int i = 0; i < K; ++i)
    {
        for (int j = 0; j < K; ++j)
        {
            seq_t sum = 0;
            for (int k = 0; k < K; ++k)
                sum += a[i][k] * b[k][j];
            res[i][j] = sum;
        }
    }
}

/* Copy matrix src to dst. */
static void mat_copy(const seq_t src[K][K], seq_t dst[K][K])
{
    for (int i = 0; i < K; ++i)
        for (int j = 0; j < K; ++j)
            dst[i][j] = src[i][j];
}

/* External function required by the test harness. */
seq_t a(idx_t n)
{
    /* Offset 0: a(0) = 1 (empty cyclic word). */
    if (n == 0)
        return (seq_t)1;

    /* Initialise the adjacency matrix once. */
    if (!base_initialized)
    {
        for (int i = 0; i < K; ++i)
            for (int j = 0; j < K; ++j)
                base[i][j] = 0;
        for (int i = 0; i < K; ++i)
        {
            base[i][i] = (seq_t)1;          /* self‑loop */
            if (i > 0)      base[i][i - 1] = (seq_t)1; /* left neighbour */
            if (i + 1 < K)  base[i][i + 1] = (seq_t)1; /* right neighbour */
        }
        base_initialized = true;
    }

    seq_t result[K][K];
    seq_t cur[K][K];
    seq_t tmp[K][K];

    /* result ← identity matrix. */
    for (int i = 0; i < K; ++i)
        for (int j = 0; j < K; ++j)
            result[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;

    /* cur ← base. */
    mat_copy(base, cur);

    /* Binary exponentiation of the matrix. */
    for (idx_t exp = n; exp > 0; exp >>= 1)
    {
        if (exp & 1)
        {
            mat_mult(result, cur, tmp);
            mat_copy(tmp, result);
        }
        mat_mult(cur, cur, tmp);
        mat_copy(tmp, cur);
    }

    /* Compute trace. */
    seq_t trace = 0;
    for (int i = 0; i < K; ++i)
        trace += result[i][i];

    return trace;
}
