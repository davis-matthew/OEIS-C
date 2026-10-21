/*
 * A237802  Number of (n+1) X (1+1) 0..2 arrays with the maximum plus the
 *          upper median minus the minimum of every 2 X 2 subblock equal.
 * https://oeis.org/A237802
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = count of (n+1)×2 arrays with values 0..2 such that for
 *            every 2×2 subblock the value (max + upper‑median – min) is the
 *            same for all subblocks. The sequence is computed by enumerating
 *            the 9 possible row patterns, building adjacency matrices for the
 *            5 possible values of the expression and cou...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 13, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   row_digit
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/row_digit.h"

/* compute the value: max + upper‑median – min for a 2×2 block given
   two rows (r1,r2) encoded 0..8. */
static int block_value(int r1, int r2)
{
    int v[4] = { row_digit(r1,0), row_digit(r1,1),
                 row_digit(r2,0), row_digit(r2,1) };
    /* simple sort of 4 elements */
    for (int i = 0; i < 3; ++i)
        for (int j = i + 1; j < 4; ++j)
            if (v[i] > v[j]) { int t = v[i]; v[i] = v[j]; v[j] = t; }
    int min = v[0];
    int max = v[3];
    int upper_median = v[2];   /* third smallest */
    return max + upper_median - min;   /* in 0..4 */
}

/* matrix type: 9×9 of seq_t */
typedef seq_t Matrix[9][9];

/* matrix multiplication: C = A * B */
static void mat_mult(const Matrix A, const Matrix B, Matrix C)
{
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < 9; ++k) {
                sum += A[i][k] * B[k][j];
            }
            C[i][j] = sum;
        }
    }
}

/* matrix exponentiation: R = M^exp (exp > 0) */
static void mat_pow(const Matrix M, idx_t exp, Matrix R)
{
    /* initialize R as identity */
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            R[i][j] = (i == j) ? 1 : 0;

    Matrix cur;
    for (int i = 0; i < 9; ++i)
        for (int j = 0; j < 9; ++j)
            cur[i][j] = M[i][j];

    while (exp) {
        if (exp & 1) {
            Matrix tmp;
            mat_mult(R, cur, tmp);
            for (int i = 0; i < 9; ++i)
                for (int j = 0; j < 9; ++j)
                    R[i][j] = tmp[i][j];
        }
        exp >>= 1;
        if (!exp) break;
        Matrix tmp;
        mat_mult(cur, cur, tmp);
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                cur[i][j] = tmp[i][j];
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n < 1) return 0;   /* undefined for n<1 */

    /* Lazy static initialization of adjacency matrices */
    static bool initialized = false;
    static Matrix adj[5];   /* 0..4 */
    if (!initialized) {
        for (int v = 0; v <= 4; ++v)
            for (int i = 0; i < 9; ++i)
                for (int j = 0; j < 9; ++j)
                    adj[v][i][j] = 0;
        for (int r1 = 0; r1 < 9; ++r1) {
            for (int r2 = 0; r2 < 9; ++r2) {
                int v = block_value(r1, r2);
                adj[v][r1][r2] = 1;
            }
        }
        initialized = true;
    }

    seq_t total = 0;
    for (int v = 0; v <= 4; ++v) {
        Matrix P;
        mat_pow(adj[v], n, P);   /* n edges */
        /* sum all entries of P */
        for (int i = 0; i < 9; ++i)
            for (int j = 0; j < 9; ++j)
                total += P[i][j];
    }
    return total;
}
