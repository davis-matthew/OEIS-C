/*
 * A230600  a(n) = Lucas(2^n - 1).
 * https://oeis.org/A230600
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(2^n - 1)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)2;

    /* Matrix M = [[1,1],[1,0]] */
    const seq_t M[4] = { (seq_t)1, (seq_t)1, (seq_t)1, 0 };
    /* Start with identity matrix a_mat = [[1,0],[0,1]] */
    seq_t a_mat[4] = { (seq_t)1, 0, 0, (seq_t)1 };

    for (idx_t i = 0; i < n; ++i) {
        /* a_mat = a_mat * a_mat */
        seq_t t0 = a_mat[0]*a_mat[0] + a_mat[1]*a_mat[2];
        seq_t t1 = a_mat[0]*a_mat[1] + a_mat[1]*a_mat[3];
        seq_t t2 = a_mat[2]*a_mat[0] + a_mat[3]*a_mat[2];
        seq_t t3 = a_mat[2]*a_mat[1] + a_mat[3]*a_mat[3];
        a_mat[0] = t0; a_mat[1] = t1; a_mat[2] = t2; a_mat[3] = t3;

        /* a_mat = a_mat * M */
        t0 = a_mat[0]*M[0] + a_mat[1]*M[2];
        t1 = a_mat[0]*M[1] + a_mat[1]*M[3];
        t2 = a_mat[2]*M[0] + a_mat[3]*M[2];
        t3 = a_mat[2]*M[1] + a_mat[3]*M[3];
        a_mat[0] = t0; a_mat[1] = t1; a_mat[2] = t2; a_mat[3] = t3;
    }

    /* The trace of the resulting matrix is the Lucas number */
    return a_mat[0] + a_mat[3];
}
