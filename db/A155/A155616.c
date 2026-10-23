/*
 * A155616  a(n) = 5^n + 4^n - 1.
 * https://oeis.org/A155616
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n + 4^n - 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   pow_exp_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_exp_seq.h"

seq_t a(idx_t n) {
    seq_t pow5 = pow_exp_seq((seq_t)5, n);
    seq_t pow4 = pow_exp_seq((seq_t)4, n);
    return pow5 + pow4 - (seq_t)1;
}
