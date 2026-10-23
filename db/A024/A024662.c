/*
 * A024662  n written in fractional base 10/7.
 * https://oeis.org/A024662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0)=0; for n>0 let d = n mod 10; a(n) = d + 10 * a( (n-d)*7/10
 *            )
 * Range:     u8 n <= 19, u16 n <= 49, u32 n <= 379, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;
    seq_t v = (seq_t)n;        /* work in seq_t arithmetic */
    seq_t result = 0;
    seq_t place  = 1;
    while (v > 0) {
        seq_t d = v % (seq_t)10;           /* least significant digit */
        result += d * place;               /* build the decimal number */
        place *= (seq_t)10;
        v = (v - d) * (seq_t)7 / (seq_t)10; /* next n for the next digit */
    }
    return result;
}
