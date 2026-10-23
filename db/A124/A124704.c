/*
 * A124704  Number of base 11 circular n-digit numbers with adjacent digits
 *          differing by 1 or less.
 * https://oeis.org/A124704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = trace( A^n )        (A is the 11×11 adjacency matrix of a path
 *         with self‑loops; A[i][j] = 1 if |i‑j| ≤ 1, else 0).
 * This computes the number of base‑11 circular n‑digit numbers whose
 * adjacent digits differ by at most one.
 * The implementation uses binary exponentiation of the matrix,
 * which is O(log n) matrix multiplications.  It works for any build
 * where seq_t supports +, *, and the usual arithmetic.
 */
#include <stdint.h>

#define SZ 11

/* The base adjacency matrix.  Rows and columns are 0‑based, representing digits 1‑11. */
static const int init_adj[SZ][SZ] = {
    {1,1,0,0,0,0,0,0,0,0,0},
    {1,1,1,0,0,0,0,0,0,0,0},
    {0,1,1,1,0,0,0,0,0,0,0},
    {0,0,1,1,1,0,0,0,0,0,0},
    {0,0,0,1,1,1,0,0,0,0,0},
    {0,0,0,0,1,1,1,0,0,0,0},
    {0,0,0,0,0,1,1,1,0,0,0},
    {0,0,0,0,0,0,1,1,1,0,0},
    {0,0,0,0,0,0,0,1,1,1,0},
    {0,0,0,0,0,0,0,0,1,1,1},
    {0,0,0,0,0,0,0,0,0,1,1}
};

/* Multiply two SZ×SZ matrices: C = A * B.  All entries are of type seq_t. */
static void mat_mul(const seq_t A[SZ][SZ], const seq_t B[SZ][SZ], seq_t C[SZ][SZ])
{
    seq_t tmp[SZ][SZ] = {0};
    for (int i = 0; i < SZ; ++i) {
        for (int k = 0; k < SZ; ++k) {
            if (A[i][k] == 0) continue;
            for (int j = 0; j < SZ; ++j) {
                /* C[i][j] += A[i][k] * B[k][j]; */
                tmp[i][j] += A[i][k] * B[k][j];
            }
        }
    }
    /* copy result to C */
    for (int i = 0; i < SZ; ++i)
        for (int j = 0; j < SZ; ++j)
            C[i][j] = tmp[i][j];
}

/* Raise matrix M to power exp, store result in R.  Uses binary exponentiation. */
static void mat_pow(const seq_t M[SZ][SZ], idx_t exp, seq_t R[SZ][SZ])
{
    /* Initialize R as identity matrix */
    for (int i = 0; i < SZ; ++i)
        for (int j = 0; j < SZ; ++j)
            R[i][j] = (i == j) ? 1 : 0;

    seq_t base[SZ][SZ];
    for (int i = 0; i < SZ; ++i)
        for (int j = 0; j < SZ; ++j)
            base[i][j] = M[i][j];

    while (exp > 0) {
        if (exp & 1) {
            seq_t tmp[SZ][SZ];
            mat_mul(R, base, tmp);
            for (int i = 0; i < SZ; ++i)
                for (int j = 0; j < SZ; ++j)
                    R[i][j] = tmp[i][j];
        }
        /* square base */
        seq_t tmp[SZ][SZ];
        mat_mul(base, base, tmp);
        for (int i = 0; i < SZ; ++i)
            for (int j = 0; j < SZ; ++j)
                base[i][j] = tmp[i][j];

        exp >>= 1;
    }
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* a(0) = 1 by definition */

    /* Convert initial adjacency matrix to seq_t type */
    static seq_t base[SZ][SZ];
    static int initialized = 0;
    if (!initialized) {
        for (int i = 0; i < SZ; ++i)
            for (int j = 0; j < SZ; ++j)
                base[i][j] = (seq_t)init_adj[i][j];
        initialized = 1;
    }

    /* Compute base^n */
    seq_t power[SZ][SZ];
    mat_pow(base, n, power);

    /* Compute trace */
    seq_t trace = 0;
    for (int i = 0; i < SZ; ++i)
        trace += power[i][i];

    return trace;
}
