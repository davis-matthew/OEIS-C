/*
 * A125384  Number of base 21 circular n-digit numbers with adjacent digits
 *          differing by 5 or less.
 * https://oeis.org/A125384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(Adj^n) for n > 0 ; a(0) = 1 Adj is the 21×21
 *            adjacency matrix with Adj[i][j] = 1 if |i-j| <= 5, else 0
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

typedef struct { seq_t m[21][21]; } mat21;

/* Set matrix to identity */
static void mat_identity(mat21 *P)
{
    for (int i = 0; i < 21; ++i)
        for (int j = 0; j < 21; ++j)
            P->m[i][j] = (i == j) ? (seq_t)1 : (seq_t)0;
}

/* Multiply matrices: R = A * B */
static void mat_mul(mat21 *R, const mat21 *A, const mat21 *B)
{
    seq_t tmp[21][21];
    for (int i = 0; i < 21; ++i)
        for (int j = 0; j < 21; ++j)
            tmp[i][j] = (seq_t)0;

    for (int i = 0; i < 21; ++i) {
        for (int k = 0; k < 21; ++k) {
            seq_t aik = A->m[i][k];
            if (aik == (seq_t)0) continue;
            for (int j = 0; j < 21; ++j) {
                tmp[i][j] += aik * B->m[k][j];
            }
        }
    }

    for (int i = 0; i < 21; ++i)
        for (int j = 0; j < 21; ++j)
            R->m[i][j] = tmp[i][j];
}

/* The main sequence function */
seq_t a(idx_t n)
{
    if (n == (idx_t)0) return (seq_t)1;

    /* Build the adjacency matrix once */
    static mat21 Adj;
    static bool adj_init = false;
    if (!adj_init) {
        for (int i = 0; i < 21; ++i) {
            for (int j = 0; j < 21; ++j) {
                int diff = (i > j) ? (i - j) : (j - i);
                Adj.m[i][j] = (diff <= 5) ? (seq_t)1 : (seq_t)0;
            }
        }
        adj_init = true;
    }

    /* matrix exponentiation: compute Adj^n */
    mat21 result;
    mat_identity(&result);

    mat21 base = Adj;
    idx_t exp = n;

    while (exp) {
        if (exp & (idx_t)1)
            mat_mul(&result, &result, &base);
        exp >>= (idx_t)1;
        if (exp) /* avoid an unnecessary square when exp becomes 0 */
            mat_mul(&base, &base, &base);
    }

    /* trace of the result */
    seq_t trace = (seq_t)0;
    for (int i = 0; i < 21; ++i)
        trace += result.m[i][i];

    return trace;
}
