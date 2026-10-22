/*
 * A002532  a(n) = 2*a(n-1) + 5*a(n-2), a(0) = 0, a(1) = 1.
 * https://oeis.org/A002532
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) + 5*a(n-2), a(0) = 0, a(1) = 1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 19, u64 n <= 37, u128 n <= 72,
 *            big any n
 * Helpers:   mat_mul, mat_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mat_mul.h"
#include "../utils/mat_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;
    
    seq_t M[2][2] = {{2, 5}, {1, 0}};
    seq_t result[2][2];
    mat_pow(result, M, n - 1);
    
    return result[0][0];
}
