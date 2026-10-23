/*
 * A125317  Number of base 7 circular n-digit numbers with adjacent digits
 *          differing by 3 or less.
 * https://oeis.org/A125317
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = trace(A^n) for n>0 (A is the 7×7 adjacency matrix of the
 *            * graph of base‑7 digits where |i−j|≤3), a(0)=1. * This
 *            implementation uses fast matrix exponentiation, O(log n) matrix
 *            * multiplications, and works for any seq_t type (built‑in or big
 *            integer)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* adjacency matrix for digits 0…6 where |i−j|≤3 */
static const seq_t ADJ[7][7] = {
    {1,1,1,1,0,0,0},
    {1,1,1,1,1,0,0},
    {1,1,1,1,1,1,0},
    {1,1,1,1,1,1,1},
    {0,1,1,1,1,1,1},
    {0,0,1,1,1,1,1},
    {0,0,0,1,1,1,1}
};

/* multiply two 7×7 matrices: R = X * Y */
static void mul_mat(const seq_t X[7][7], const seq_t Y[7][7], seq_t R[7][7])
{
    seq_t tmp[7][7] = {0};
    for (int i = 0; i < 7; ++i) {
        for (int k = 0; k < 7; ++k) {
            seq_t xik = X[i][k];
            if (!xik) continue;
            for (int j = 0; j < 7; ++j) {
                tmp[i][j] += xik * Y[k][j];
            }
        }
    }
    for (int i = 0; i < 7; ++i)
        for (int j = 0; j < 7; ++j)
            R[i][j] = tmp[i][j];
}

/* fast exponentiation of a 7×7 matrix: set res = base^exp */
static void pow_mat(idx_t exp, const seq_t base[7][7], seq_t res[7][7])
{
    /* identity matrix in res */
    for (int i = 0; i < 7; ++i)
        for (int j = 0; j < 7; ++j)
            res[i][j] = (i == j) ? 1 : 0;

    seq_t cur[7][7];
    for (int i = 0; i < 7; ++i)
        for (int j = 0; j < 7; ++j)
            cur[i][j] = base[i][j];

    idx_t e = exp;
    while (e) {
        if (e & 1) mul_mat(res, cur, res);
        mul_mat(cur, cur, cur);
        e >>= 1;
    }
}

/* OEIS sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 1;       /* base case */

    seq_t mat[7][7];
    pow_mat(n, ADJ, mat);

    seq_t trace = 0;
    for (int i = 0; i < 7; ++i) trace += mat[i][i];
    return trace;
}
