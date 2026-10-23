/*
 * A048473  a(0)=1, a(n) = 3*a(n-1) + 2; a(n) = 2*3^n - 1.
 * https://oeis.org/A048473
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * 3^n - 1
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 39, u128 n <= 80,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t p = pow3(n);      /* p = 3^n */
    return (seq_t)2 * p - 1; /* a(n) = 2*3^n - 1 */
}
