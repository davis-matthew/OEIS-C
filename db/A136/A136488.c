/*
 * A136488  a(n) = 2^n - A005418(n).
 * https://oeis.org/A136488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^(n-2) - 2^(n/2-1) for n even a(n) = 3*2^(n-2) -
 *            2^((n-3)/2) for n odd a(1) = 1
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* handle offset 1 special case */
    if (n == 1) {
        return (seq_t)1;
    }

    seq_t pow_n_minus_2 = pow2(n - 2);
    seq_t result = pow_n_minus_2 * (seq_t)3;

    if ((n & 1) == 0) {            /* n even */
        seq_t pow_half_minus_1 = pow2((n >> 1) - 1);
        result = result - pow_half_minus_1;
    } else {                       /* n odd */
        seq_t pow_half_minus_1 = pow2((n - 3) / 2);
        result = result - pow_half_minus_1;
    }

    return result;
}
