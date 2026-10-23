/*
 * A040037  Continued fraction for sqrt(44).
 * https://oeis.org/A040037
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult
 * Formula:   not stated
 * Range:     u8 n <= 9215, u16 n <= 9215, u32 n <= 9215, u64 n <= 9215,
 *            u128 n <= 9215, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(0) = 6;
 *          for n > 0:
 *              - if n is odd or n = 2  -> a(n) = 1
 *              - if n is divisible by 4 but not by 8 -> a(n) = 2
 *              - if n is divisible by 8          -> a(n) = 12
 *          (This follows from the multiplicative definition
 *           with a(2) = 1, a(4) = 2, a(2^e) = 12 for e ≥ 3,
 *           and a(p^e) = 1 for odd primes p.)
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)6;

    /* Count exponent of 2 in n */
    idx_t e = 0;
    /* n is unsigned, so never negative */
    while ((n & 1ULL) == 0) {
        e++;
        n >>= 1;
        if (n == 0) /* defensive: n became 0 after shifting */
            break;
    }

    if (e < 2)
        return (seq_t)1;   /* e==0 (odd) or e==1 (exactly 2) */
    if (e == 2)
        return (seq_t)2;
    /* e >= 3 */
    return (seq_t)12;
}
