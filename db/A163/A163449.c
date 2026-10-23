/*
 * A163449  A007582 written in base 2.
 * https://oeis.org/A163449
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A163449
   a(0) = 1
   for n >= 1: a(n) = 10^(2*n-1) + 10^(n-1)
   (That is, a string '1', (n-1) zeros, '1', (n-1) zeros in decimal.)
*/
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        return (seq_t)1;
    }
    /* compute 10^(2*n-1) and 10^(n-1) using the provided big‑int helper */
    seq_t high = pow10(2*n - 1);
    seq_t low  = pow10(n - 1);
    return high + low;
}
