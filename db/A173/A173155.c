/*
 * A173155  a(n) = binomial(n + 5, 5) * 8^n.
 * https://oeis.org/A173155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n + 5, 5)*8^n
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 36,
 *            big any n
 * Helpers:   binomial, pow_uint, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"
#include "../utils/pow_uint.h"
#include "../utils/seq_mul.h"

seq_t a(idx_t n) {
    seq_t bin = binomial(n + 5, 5);
    seq_t pow = pow_uint(8, n);
    return seq_mul(bin, pow);
}
