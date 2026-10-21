/*
 * A201630  a(n) = a(n-1) + 2*a(n-2) with n>1, a(0)=2, a(1)=7.
 * https://oeis.org/A201630
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^n - (-1)^n (a(0)=2, a(1)=7)
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute 2^n by repeated left shift (O(n), fine for n ≤ 10000) */
    seq_t pow = (seq_t)1;
    for (idx_t i = 0; i < n; ++i)
        pow <<= 1;        /* multiply by 2 each iteration */

    /* 3*2^n */
    seq_t res = (seq_t)3 * pow;

    /* subtract (-1)^n: if n is odd add 1, else subtract 1 */
    if (n & 1ULL)
        res += (seq_t)1;
    else
        res -= (seq_t)1;

    return res;
}
