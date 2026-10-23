/*
 * A138144  Palindromes formed from the reflected decimal expansion of the
 *          concatenation of 1, 1 and infinite 0's.
 * https://oeis.org/A138144
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(1)=1, a(2)=11, a(3)=111, a(n)=11+11*10^(n-2) for n>3
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 20, u128 n <= 21,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 1) return 1;
    if (n == 2) return 11;
    if (n == 3) return 111;
    seq_t p = pow10(n - 2);
    return 11 + 11 * p;
}
