/*
 * A136863  Numbers k such that k and k^2 use only the digits 0, 1, 4 and 7.
 * https://oeis.org/A136863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function returns the A136863 sequence:
 * a(1)=0, a(2)=1, a(3)=10, a(4)=100.
 * For n ≥ 5:
 *   if n is odd: a(n) = 10^((n+1)/2)
 *   if n is even: a(n) = 1071 * 10^((n-6)/2)   */
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)1;
    if (n == 3) return (seq_t)10;
    if (n == 4) return (seq_t)100;

    if (n & 1) {                 /* odd n */
        return pow10((n + 1) / 2);
    } else {                     /* even n */
        return pow10((n - 6) / 2) * (seq_t)1071;
    }
}
