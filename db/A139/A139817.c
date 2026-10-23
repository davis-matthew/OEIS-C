/*
 * A139817  2^n - number of digits of 2^n.
 * https://oeis.org/A139817
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = 2^n - number of decimal digits of 2^n
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   digit_len, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/digit_len.h"

seq_t a(idx_t n)
{
    seq_t p = pow2(n);
    int d = digit_len(p);
    return p - (seq_t)d;
}
