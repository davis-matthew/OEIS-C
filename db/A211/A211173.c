/*
 * A211173  a(n) = (2n)!^n (modulo 2n+1).
 * https://oeis.org/A211173
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((2*n)!^n) mod (2*n+1)
 * Range:     u8 n <= 130, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* (2*n)! mod (2*n+1) is -1 when 2*n+1 is prime, otherwise 0.
       Thus a(n) = (-1)^n (mod 2*n+1) for prime modulus.
       For prime modulus the result is 1 when n is even,
       and 2*n when n is odd (i.e., 2*n+1 - 1).  For composite
       modulus the result is 0. */
    idx_t p = 2*n + 1;          /* modulus */
    if (is_prime(p)) {
        if ((n & 1) == 0)        /* n even */
            return (seq_t)1;
        else                    /* n odd */
            return (seq_t)(p - 1); /* = 2*n */
    } else
        return (seq_t)0;
}
