/*
 * A040270  Continued fraction for sqrt(287).
 * https://oeis.org/A040270
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   a(n) = 16 if n = 0; otherwise * a(n) = 15 if n is even but not
 *            divisible by 4, * a(n) = 32 if n is divisible by 4, * a(n) = 1
 *            if n is odd. * This follows from the multiplicative definition
 *            of A040270 * (a(2)=15, a(2^e)=32 for e≥2, a(p^e)=1 for primes
 *            p≥3)
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t if needed */
 
seq_t a(idx_t n)
{
    if (n == 0)                /* special offset */
        return (seq_t)16;
    if ((n & 1ULL) == 0) {     /* n is even */
        if ((n & 3ULL) == 0)   /* divisible by 4 */
            return (seq_t)32;
        else                  /* exactly one factor 2 */
            return (seq_t)15;
    }
    return (seq_t)1;            /* odd numbers */
}
