/*
 * A000178  Superfactorials: product of first n factorials.
 * https://oeis.org/A000178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = n! * a(n-1), a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 6, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    seq_t f = 1;   /* running factorial i! */
    seq_t r = 1;   /* running product 1!*2!*...*i! */
    idx_t i;
    for (i = 1; i <= n; i++) {
        f *= (seq_t)i;
        r *= f;
    }
    return r;
}
