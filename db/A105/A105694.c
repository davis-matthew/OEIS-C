/*
 * A105694  a(n) = ceiling(10^n - 10^(n-2)).
 * https://oeis.org/A105694
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10^n - 10^(n-2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* a(0) = 1, a(1) = 10, and for n >= 2:  a(n) = 99 * 10^(n-2) */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)10;
    return (seq_t)(99 * pow10(n - 2));
}
