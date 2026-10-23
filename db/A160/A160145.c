/*
 * A160145  a(n) = the odd number 2n+1 minus the numerator of
 *          (2n+1)/(2^(2n+1)-1).
 * https://oeis.org/A160145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n+1) - (2n+1)/gcd(2n+1, 2^(2n+1)-1)
 * Range:     u8 n <= 156, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Which is the same as a(n) = A005408(n) - A160144(n)        */

#include <stdint.h>
#include "../utils/gcd.h"
#include "../utils/mod_pow.h"

/* Returns the n-th term of sequence A160145 */
seq_t a(idx_t n)
{
    /* odd number 2n+1 */
    seq_t p = (seq_t)n * 2 + 1;
    /* exponent 2n+1 for the modular power */
    idx_t exp = 2 * n + 1;

    /* 2^(2n+1) mod p */
    seq_t a_mod = mod_pow((seq_t)2, exp, p);

    /* small congruence of the numerator denominator difference */
    seq_t m = (a_mod == 0) ? (p - 1) : (a_mod - 1);

    /* g = gcd(p, a_mod - 1) */
    seq_t g = gcd(p, m);

    /* numerator of the reduced fraction (2n+1)/g */
    seq_t num = p / g;

    /* return (2n+1) - numerator */
    return p - num;
}
