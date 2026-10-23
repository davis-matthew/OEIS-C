/*
 * A125347  Number of base 10 circular n-digit numbers with adjacent digits
 *          differing by 4 or less.
 * https://oeis.org/A125347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) for n ≥ 1, a(0) = 1 * * where A is the 10×10
 *            adjacency matrix of the graph on vertices * {0,…,9} with an
 *            undirected edge between i and j iff |i–j| ≤ 4. * The number of
 *            closed walks of length n starting and ending at an * arbitrary
 *            vertex equals the trace of A^n, and each such wa...
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

/* Multiply two 10×10 matrices a and b, store the result in out. */
static void mat_mul(seq_t out[10][10],
                    const seq_t a[10][10],
                    const seq_t b[10][10])
{
    seq_t temp[10][10];
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            seq_t sum = (seq_t)0;
            for (int k = 0; k < 10; ++k) {
                sum += a[i][k] * b[k][j];
            }
            temp[i][j] = sum;
        }
    }
    /* copy the temporary result back to out */
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            out[i][j] = temp[i][j];
        }
    }
}

/* Adjacency matrix for |i–j| ≤ 4, symmetric and including loops */
static const seq_t ADJ[10][10] = {
    {1,1,1,1,1,0,0,0,0,0},
    {1,1,1,1,1,1,0,0,0,0},
    {1,1,1,1,1,1,1,0,0,0},
    {1,1,1,1,1,1,1,1,0,0},
    {1,1,1,1,1,1,1,1,1,0},
    {0,1,1,1,1,1,1,1,1,1},
    {0,0,1,1,1,1,1,1,1,1},
    {0,0,0,1,1,1,1,1,1,1},
    {0,0,0,0,1,1,1,1,1,1},
    {0,0,0,0,0,1,1,1,1,1}
};

seq_t a(idx_t n)
{
    if (n == 0)            /* a(0) = 1 by definition */
        return (seq_t)1;

    /* Fast exponentiation of the adjacency matrix. */
    seq_t result[10][10];
    seq_t base[10][10];

    /* initialize result as the identity matrix */
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            result[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
            base[i][j]   = ADJ[i][j];
        }
    }

    idx_t power = n;
    while (power > 0) {
        if (power & (idx_t)1) {
            mat_mul(result, result, base);
        }
        mat_mul(base, base, base);
        power >>= 1;
    }

    /* trace of A^n */
    seq_t trace = (seq_t)0;
    for (int i = 0; i < 10; ++i) {
        trace += result[i][i];
    }
    return trace;
}
