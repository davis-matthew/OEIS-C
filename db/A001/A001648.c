/*
 * A001648  Tetranacci numbers A073817 without the leading term 4.
 * https://oeis.org/A001648
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = trace of M^n, where M = [[0,1,0,0], [0,0,1,0], [0,0,0,1],
 *            [1,1,1,1]], implementing a(n) = a(n-1) + a(n-2) + a(n-3) +
 *            a(n-4)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 33, u64 n <= 67, u128 n <= 135,
 *            big any n
 * Helpers:   mat4_mul, mat4_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mat4_mul.h"
#include "../utils/mat4_pow.h"

seq_t a(idx_t n) {
    seq_t M[4][4];
    M[0][0] = 0; M[0][1] = 1; M[0][2] = 0; M[0][3] = 0;
    M[1][0] = 0; M[1][1] = 0; M[1][2] = 1; M[1][3] = 0;
    M[2][0] = 0; M[2][1] = 0; M[2][2] = 0; M[2][3] = 1;
    M[3][0] = 1; M[3][1] = 1; M[3][2] = 1; M[3][3] = 1;
    
    seq_t Mn[4][4];
    mat4_pow(Mn, M, n);
    
    return Mn[0][0] + Mn[1][1] + Mn[2][2] + Mn[3][3];
}
