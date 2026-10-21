/*
 * A080095  Let sum(k>=0, k^n/(2*k+1)!) = (x(n)*e + y(n)/e)/z(n), where x(n)
 *          and z(n) are >0, then a(n)=z(n).
 * https://oeis.org/A080095
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^b(n), where b(n) = n if n mod 3 == 1, n+1 if n mod 3 ==
 *            0 or 2
 * Range:     u8 n <= 7, u16 n <= 14, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute b(n) */
    idx_t b = (n % 3 == 1) ? n : n + 1;

    /* compute 2^b */
    seq_t result = 1;
    for (idx_t i = 0; i < b; ++i)
        result *= (seq_t)2;

    return result;
}
