/*
 * A124710  Number of base 17 circular n-digit numbers with adjacent digits
 *          differing by 1 or less.
 * https://oeis.org/A124710
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 19, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A124710: Number of base 17 circular n‑digit numbers with adjacent digits
 * differing by 1 or less.  The values are given by a(n) = trace(A^n), where A is
 * the 17×17 adjacency matrix of a path graph with self‑loops: A[i][i]=1,
 * A[i][i±1]=1 for 0 ≤ i ≤ 16, and A[i][j]=0 otherwise.  This implementation
 * computes a(n) by binary exponentiation of A, which runs in O(17³ log n) time
 * and uses only integer arithmetic on type seq_t. */

#include <stddef.h>

/* Multiply two 17×17 matrices: res = a * b */
static void mat_mult(const seq_t a[17][17], const seq_t b[17][17], seq_t res[17][17])
{
    seq_t temp[17][17] = {0};
    for (idx_t i = 0; i < 17; ++i) {
        for (idx_t k = 0; k < 17; ++k) {
            seq_t aik = a[i][k];
            if (aik == 0) continue;
            for (idx_t j = 0; j < 17; ++j) {
                temp[i][j] += aik * b[k][j];
            }
        }
    }
    for (idx_t i = 0; i < 17; ++i)
        for (idx_t j = 0; j < 17; ++j)
            res[i][j] = temp[i][j];
}

/* Compute A^n via binary exponentiation; the result is stored in res */
static void mat_pow(idx_t n, seq_t res[17][17])
{
    /* Initialise res as the identity matrix */
    for (idx_t i = 0; i < 17; ++i)
        for (idx_t j = 0; j < 17; ++j)
            res[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;

    /* Build the base matrix A once */
    seq_t base[17][17] = {0};
    for (idx_t i = 0; i < 17; ++i) {
        base[i][i] = (seq_t)1;                /* self-loop */
        if (i > 0)   base[i][i - 1] = (seq_t)1;/* left neighbour */
        if (i < 16)  base[i][i + 1] = (seq_t)1;/* right neighbour */
    }

    while (n) {
        if (n & 1) {
            seq_t tmp[17][17] = {0};
            mat_mult(res, base, tmp);
            for (idx_t i = 0; i < 17; ++i)
                for (idx_t j = 0; j < 17; ++j)
                    res[i][j] = tmp[i][j];
        }
        n >>= 1;
        if (n) {
            seq_t tmp[17][17] = {0};
            mat_mult(base, base, tmp);
            for (idx_t i = 0; i < 17; ++i)
                for (idx_t j = 0; j < 17; ++j)
                    base[i][j] = tmp[i][j];
        }
    }
}

/* Public function as required */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;   /* a(0) = 1 by definition */
    seq_t mat[17][17];
    mat_pow(n, mat);
    seq_t trace = (seq_t)0;
    for (idx_t i = 0; i < 17; ++i) trace += mat[i][i];
    return trace;
}
