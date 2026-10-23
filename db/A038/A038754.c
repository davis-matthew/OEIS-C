/*
 * A038754  a(2n) = 3^n, a(2n+1) = 2*3^n.
 * https://oeis.org/A038754
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(2*n) = 3^n, a(2*n+1) = 2*3^n
 * Range:     u8 n <= 10, u16 n <= 20, u32 n <= 40, u64 n <= 80, u128 n <= 161,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Check parity of n */
    if ((n & 1ULL) == 0) {
        /* n is even: a(2n) = 3^n */
        return pow3(n / 2);
    } else {
        /* n is odd: a(2n+1) = 2*3^n */
        return (seq_t)2 * pow3((n - 1) / 2);
    }
}
