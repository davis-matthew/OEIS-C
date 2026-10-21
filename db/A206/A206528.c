/*
 * A206528  5^n concatenated with itself.
 * https://oeis.org/A206528
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = A020338(5^n) (concatenate 5^n with itself)
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 6, u64 n <= 13, u128 n <= 27,
 *            big any n
 * Helpers:   big_pow, digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Compute 5^n */
    seq_t v = big_pow((seq_t)5, (seq_t)n);
    /* Number of decimal digits of v */
    idx_t d = (idx_t)digit_len(v);
    /* 10^d */
    seq_t p = pow10(d);
    /* Concatenate v with itself: v * 10^d + v */
    return v * p + v;
}
