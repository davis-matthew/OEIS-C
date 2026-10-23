/*
 * A198971  a(n) = 5*10^n - 1.
 * https://oeis.org/A198971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*10^n - 1
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t p = pow10(n);   /* p = 10^n */
    return 5 * p - 1;     /* a(n) = 5*10^n - 1 */
}
