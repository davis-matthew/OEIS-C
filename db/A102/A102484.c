/*
 * A102484  Numbers of the concatenated form 9nn9.
 * https://oeis.org/A102484
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 9*(1 + 10^(2*A055642(n-1)+1)) +
 *            (n-1)*(10+10^(1+A055642(n-1)))
 * Range:     u8 none, u16 n <= 10, u32 n <= 1000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

static inline idx_t digit_len(idx_t x)
{
    /* return number of decimal digits of x, treating 0 as having 1 digit */
    if (x == 0) return 1;
    idx_t d = 0;
    while (x) {
        ++d;
        x /= 10;
    }
    return d;
}

seq_t a(idx_t n)
{
    /* a(1) corresponds to the first printed term */
    idx_t m = n - 1;                     /* n-1, safe for n>=1 */
    idx_t d = digit_len(m);              /* digits of n-1 */
    seq_t p1 = pow10(2*d + 1);           /* 10^(2*d+1) */
    seq_t term1 = 9 * (1 + p1);          /* 9*(1+10^(2*d+1)) */
    seq_t p2 = pow10(1 + d);             /* 10^(1+d)  */
    seq_t term2 = (seq_t)m * (10 + p2);  /* (n-1)*(10+10^(1+d)) */
    return term1 + term2;
}
