/*
 * A011583  Legendre symbol (n,13).
 * https://oeis.org/A011583
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   a(n) = (n|13), the Legendre symbol modulo 13. a(n) = 0 if 13
 *            divides n, a(n) = 1 if n is a quadratic residue mod 13, a(n) =
 *            -1 otherwise. The sequence is completely multiplicative with
 *            a(13)=0
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   pow_mod13
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_mod13.h"

seq_t a(idx_t n)
{
    if (n % 13 == 0)
        return 0;          /* n is divisible by 13 */

    seq_t b = (seq_t)(n % 13);      /* reduced modulo 13 */
    seq_t r = pow_mod13(b, 6);      /* Euler's criterion for p=13 */

    return (r == 1) ? (seq_t)1 : (seq_t)-1;
}
