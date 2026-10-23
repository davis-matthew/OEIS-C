/*
 * A108213  a(0)=44; if n odd, a(n) = a(n-1)/2, otherwise a(n) = 4*a(n-1).
 * https://oeis.org/A108213
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=44, a(n)=a(n-1)/2 if n is odd, a(n)=4*a(n-1) if n is even.
 *            Equivalent closed form: a(2m)=44*2^m and a(2m+1)=22*2^m
 * Range:     u8 n <= 5, u16 n <= 21, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the trivial base case */
    if (n == 0)
        return (seq_t)44;

    /* m is the exponent of 2 to multiply the base value (22 or 44) */
    idx_t m = n / 2;
    seq_t res;

    if (n & 1)          /* odd index */
        res = (seq_t)22;/* a(2m+1) = 22 * 2^m */
    else                /* even index */
        res = (seq_t)44;/* a(2m)   = 44 * 2^m */

    /* Multiply by 2^m without using a shift that may be undefined for
       large exponents.  Adding the value to itself is equivalent to a
       multiplication by 2 and works for all provided seq_t types. */
    while (m--)
        res += res;

    return res;
}
