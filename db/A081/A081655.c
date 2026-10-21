/*
 * A081655  a(n) = 2*5^n-1.
 * https://oeis.org/A081655
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*5^n - 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   pow_of_5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow_of_5.h"

seq_t a(idx_t n) {
    seq_t p = pow_of_5(n);
    return p * 2 - 1;
}
