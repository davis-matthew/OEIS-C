/*
 * A124703  Number of base 10 circular n-digit numbers with adjacent digits
 *          differing by 1 or less.
 * https://oeis.org/A124703
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) for n ≥ 1, a(0) = 1 where A is the 10×10
 *            adjacency matrix of the graph with vertices 1..10 and edges
 *            between vertices that differ by at most 1
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

static void mat_mult(const seq_t a[10][10], const seq_t b[10][10], seq_t out[10][10])
{
    for (int i = 0; i < 10; ++i) {
        for (int j = 0; j < 10; ++j) {
            seq_t sum = 0;
            for (int k = 0; k < 10; ++k) {
                sum += a[i][k] * b[k][j];
            }
            out[i][j] = sum;
        }
    }
}

static void mat_copy(const seq_t src[10][10], seq_t dst[10][10])
{
    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 10; ++j)
            dst[i][j] = src[i][j];
}

seq_t a(idx_t n)
{
    if (n == 0) return 1;          /* a(0) = 1 */

    /* adjacency matrix A[i][j] = 1 if |i-j| <= 1 otherwise 0 */
    static const seq_t A_init[10][10] = {
        {1,1,0,0,0,0,0,0,0,0},
        {1,1,1,0,0,0,0,0,0,0},
        {0,1,1,1,0,0,0,0,0,0},
        {0,0,1,1,1,0,0,0,0,0},
        {0,0,0,1,1,1,0,0,0,0},
        {0,0,0,0,1,1,1,0,0,0},
        {0,0,0,0,0,1,1,1,0,0},
        {0,0,0,0,0,0,1,1,1,0},
        {0,0,0,0,0,0,0,1,1,1},
        {0,0,0,0,0,0,0,0,1,1}
    };

    seq_t res[10][10];   /* result matrix (accumulator) */
    seq_t base[10][10];  /* base matrix for exponentiation */
    seq_t temp[10][10];  /* temporary matrix */

    /* initialize res as identity matrix */
    for (int i = 0; i < 10; ++i)
        for (int j = 0; j < 10; ++j)
            res[i][j] = (i == j) ? 1 : 0;

    /* initialize base with A */
    mat_copy(A_init, base);

    idx_t exp = n;
    while (exp > 0) {
        if (exp & 1) {
            mat_mult(res, base, temp);
            mat_copy(temp, res);
        }
        mat_mult(base, base, temp);
        mat_copy(temp, base);
        exp >>= 1;
    }

    /* compute trace of res (which is A^n) */
    seq_t trace = 0;
    for (int i = 0; i < 10; ++i)
        trace += res[i][i];

    return trace;
}
