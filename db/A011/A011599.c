/*
 * A011599  Legendre symbol (n,79).
 * https://oeis.org/A011599
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, mult, easy
 * Formula:   not stated
 * Range:     i8 n <= 10000, i16 n <= 10000, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS Formula: a(n) = n^39 (mod 79) mapped to the Legendre symbol (n,79)
 *  Returns  1  if n^39 ≡  1 (mod 79)
 *          -1  if n^39 ≡ -1 (mod 79)
 *           0  if 79 divides n
 *
 *  The sequence is the Legendre symbol (n,79) for all n ≥ 0.
 */
#include <stdint.h>

seq_t a(idx_t n)
{
    /* Modulus 79 */
    const uint64_t m = 79ULL;

    /* Compute base = n mod 79 in seq_t */
    seq_t base = (seq_t)(n % m);
    seq_t result = (seq_t)1;
    uint64_t exp = 39ULL;           /* (79-1)/2 */

    /* Binary exponentiation of base^exp mod 79 */
    while (exp) {
        if (exp & 1ULL)
            result = (result * base) % (seq_t)m;
        base = (base * base) % (seq_t)m;
        exp >>= 1ULL;
    }

    /* Map to Legendre symbol values */
    if ((uint64_t)result == 1ULL)       return (seq_t)1;
    if ((uint64_t)result == m - 1ULL)   return (seq_t)-1;
    return (seq_t)0;                    /* n is a multiple of 79 */
}
