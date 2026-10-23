/*
 * A173764  a(n) = (10^n+17)/3.
 * https://oeis.org/A173764
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^n + 17)/3, offset 1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* a(1) = (10^1 + 17)/3 = 9, a(2) = 39, ... */
    if (n == 0) {
        return (seq_t)0;          /* not part of the sequence */
    }
    seq_t ten_pow = pow10(n);    /* 10^n */
    seq_t result = (ten_pow + (seq_t)17) / (seq_t)3;
    return result;
}
