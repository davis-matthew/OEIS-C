/*
 * A125441  Number of base 29 circular n-digit numbers with adjacent digits
 *          differing by 7 or less.
 * https://oeis.org/A125441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace( A^n ) where A is the 29×29 adjacency matrix with
 *            A[i][j] = 1 if |i-j| ≤ 7, otherwise 0. a(0) is defined as 1. For
 *            n ≥ 1, a(n) is the number of closed walks of length n in this
 *            graph, equivalently the number of base‑29 circular n‑digit
 *            numbers whose adjacent digits differ by at most...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

/* constants */
#define B 29            /* base size (number of digits) */
#define D 7             /* maximal allowed difference between adjacent digits */

/* multiplication of two B×B matrices of type seq_t
   result C = A · B   */
static void mat_mult(const seq_t A[B][B], const seq_t Bm[B][B], seq_t C[B][B])
{
    seq_t sum;
    for (idx_t i = 0; i < B; ++i) {
        for (idx_t j = 0; j < B; ++j) {
            sum = 0;
            for (idx_t k = 0; k < B; ++k) {
                sum += A[i][k] * Bm[k][j];
            }
            C[i][j] = sum;
        }
    }
}

/* exponentiation by squaring: res = M^exp  (M is B×B matrix) */
static void mat_pow(const seq_t M[B][B], idx_t exp, seq_t res[B][B])
{
    seq_t identity[B][B];
    seq_t base[B][B];
    seq_t tmp[B][B];

    /* identity matrix */
    for (idx_t i = 0; i < B; ++i) {
        for (idx_t j = 0; j < B; ++j) {
            identity[i][j] = (i == j) ? 1 : 0;
        }
    }

    /* copy identity to result */
    for (idx_t i = 0; i < B; ++i)
        for (idx_t j = 0; j < B; ++j)
            res[i][j] = identity[i][j];

    /* copy base matrix */
    for (idx_t i = 0; i < B; ++i)
        for (idx_t j = 0; j < B; ++j)
            base[i][j] = M[i][j];

    while (exp > 0) {
        if (exp & 1) {
            mat_mult(res, base, tmp);
            for (idx_t i = 0; i < B; ++i)
                for (idx_t j = 0; j < B; ++j)
                    res[i][j] = tmp[i][j];
        }
        mat_mult(base, base, tmp);
        for (idx_t i = 0; i < B; ++i)
            for (idx_t j = 0; j < B; ++j)
                base[i][j] = tmp[i][j];
        exp >>= 1;
    }
}

/* adjacency matrix for the graph of allowed digit transitions */
static void init_adjacency(seq_t adj[B][B])
{
    for (idx_t i = 0; i < B; ++i) {
        for (idx_t j = 0; j < B; ++j) {
            idx_t diff = (i > j) ? i - j : j - i;
            adj[i][j] = (diff <= D) ? 1 : 0;
        }
    }
}

/* the required function for the OEIS sequence A125441 */
seq_t a(idx_t n)
{
    /* a(0) is defined as 1 */
    if (n == 0)
        return 1;

    static seq_t adjacency[B][B];
    static bool initialized = false;

    if (!initialized) {
        init_adjacency(adjacency);
        initialized = true;
    }

    seq_t pow_mat[B][B];
    mat_pow(adjacency, n, pow_mat);

    seq_t trace = 0;
    for (idx_t i = 0; i < B; ++i)
        trace += pow_mat[i][i];

    return trace;
}
