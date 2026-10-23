/*
 * A178664  a(n) = 2^n concatenated with itself.
 * https://oeis.org/A178664
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 2^n concatenated with 2^n
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 15, u64 n <= 30, u128 n <= 63,
 *            big any n
 * Helpers:   digit_len, pow10, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/pow10.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    /* Compute 2^n */
    seq_t v = pow2(n);

    /* Number of decimal digits in 2^n */
    int d = digit_len(v);

    /* 10^d */
    seq_t pow10_d = pow10((idx_t)d);

    /* Concatenation: v * 10^d + v */
    return v * pow10_d + v;
}
