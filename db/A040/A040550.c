/*
 * A040550  Continued fraction for sqrt(574).
 * https://oeis.org/A040550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, less, mult
 * Formula:   a(0)=23; for n >= 1: a(n) = 1 if n is odd, a(n) = 22 if n is
 *            divisible by 2 but not by 4 (exactly one factor 2), a(n) = 46 if
 *            n is divisible by 4 (two or more factors of 2). This follows
 *            from the multiplicative definition a(2)=22, a(2^e)=46 for e>=2,
 *            and a(p^e)=1 for odd primes p
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the offset term a(0). */
    if (n == 0) {
        return (seq_t)23;
    }

    /* Count the exponent of 2 dividing n (v2(n)). */
    idx_t temp = n;
    unsigned int v2 = 0;
    while ((temp & 1ULL) == 0) {
        temp >>= 1;
        ++v2;
    }

    /* Determine the value based on v2(n). */
    if (v2 == 0) {
        return (seq_t)1;        /* n is odd */
    } else if (v2 == 1) {
        return (seq_t)22;       /* n has exactly one factor 2 */
    } else {
        return (seq_t)46;       /* n divisible by 4 (two or more factors) */
    }
}
