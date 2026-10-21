/*
 * A056455  Palindromes using exactly four different symbols.
 * https://oeis.org/A056455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4^⌊(n+1)/2⌋ - 4*3^⌊(n+1)/2⌋ + 6*2^⌊(n+1)/2⌋ - 4
 * Range:     u8 n <= 10, u16 n <= 16, u32 n <= 32, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   pow_int
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* (equivalent to 4!·Stirling2(⌊(n+1)/2⌋,4)) */

#include "../utils/pow_int.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n<1, return 0 */
        return 0;
    idx_t m = (n + 1) / 2;    /* floor((n+1)/2) */
    seq_t p4 = pow_int((seq_t)4, m);
    seq_t p3 = pow_int((seq_t)3, m);
    seq_t p2 = pow_int((seq_t)2, m);
    seq_t res = p4;
    res -= (seq_t)4 * p3;
    res += (seq_t)6 * p2;
    res -= (seq_t)4;   /* subtract 4*1^m */
    return res;
}
