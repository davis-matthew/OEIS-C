/*
 * A100896  a(n) has decimal expansion 3nn3.
 * https://oeis.org/A100896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 3*(1+10^(2*d+1)) + n*(10+10^(d+1)) where d = number of
 *            decimal digits of n (d = A055642(n))
 * Range:     u8 none, u16 n <= 9, u32 n <= 9999, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digits, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digits.h"
#include "../utils/pow10.h"

seq_t a(idx_t n) {
    idx_t d = digits(n);                     /* number of digits of n */
    seq_t ten_pow_2d_plus1 = pow10(2 * d + 1);/* 10^(2*d+1)          */
    seq_t ten_pow_d_plus1 = pow10(d + 1);     /* 10^(d+1)             */
    seq_t term1 = (seq_t)3 * ((seq_t)1 + ten_pow_2d_plus1);
    seq_t term2 = (seq_t)n * ((seq_t)10 + ten_pow_d_plus1);
    return term1 + term2;
}
