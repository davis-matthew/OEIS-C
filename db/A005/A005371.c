/*
 * A005371  a(n) = L(L(n)), where L(n) are Lucas numbers A000032.
 * https://oeis.org/A005371
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = L(L(n)), where L(n) are Lucas numbers
 * Range:     u8 n <= 5, u16 n <= 6, u32 n <= 7, u64 n <= 9, u128 n <= 10,
 *            big any n
 * Helpers:   matrix_mult
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/matrix_mult.h"

static seq_t lucas(seq_t n) {
    if (n == 0) return (seq_t)2;
    if (n == 1) return (seq_t)1;
    
    seq_t M[2][2] = {{(seq_t)1, (seq_t)1}, {(seq_t)1, (seq_t)0}};
    seq_t result[2][2] = {{(seq_t)1, (seq_t)0}, {(seq_t)0, (seq_t)1}};
    
    while (n > 0) {
        if (n % 2 == 1) {
            seq_t temp[2][2];
            matrix_mult(result, M, temp);
            result[0][0] = temp[0][0];
            result[0][1] = temp[0][1];
            result[1][0] = temp[1][0];
            result[1][1] = temp[1][1];
        }
        seq_t temp[2][2];
        matrix_mult(M, M, temp);
        M[0][0] = temp[0][0];
        M[0][1] = temp[0][1];
        M[1][0] = temp[1][0];
        M[1][1] = temp[1][1];
        n = n / 2;
    }
    
    return result[0][1] + (seq_t)2 * result[1][1];
}

seq_t a(idx_t n) {
    seq_t lucas_n = lucas((seq_t)n);
    return lucas(lucas_n);
}
