/*
 * A177114  a(n) = 4*(10^n-9).
 * https://oeis.org/A177114
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*(10^n - 9)
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    seq_t p = pow10(n);          /* 10^n */
    seq_t res = p - (seq_t)9;    /* 10^n - 9 */
    res *= (seq_t)4;             /* 4*(10^n - 9) */
    return res;
}
