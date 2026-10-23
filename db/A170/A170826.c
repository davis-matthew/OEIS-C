/*
 * A170826  a(n) = gcd(n^2, n!).
 * https://oeis.org/A170826
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n if n is prime; else n^2 (except a(4)=8)
 * Range:     u8 n <= 15, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* 1 maps to 1 automatically */
    if (is_prime(n))
        return (seq_t)n;              /* prime case */
    if (n == 4)
        return (seq_t)8;              /* special composite case */
    seq_t t = (seq_t)n;
    return t * t;                     /* generic composite case */
}
