/*
 * A167350  Totally multiplicative sequence with a(p) = (p+1)*(p-2) = p^2-p-2
 *          for prime p.
 * https://oeis.org/A167350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = \prod_{p^e||n} ((p+1)(p-2))^e (zero if n is even)
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* a(1) = 1, a(2) = 0 */
    if (n < 1) return (seq_t)0;
    if ((n & 1) == 0) return (seq_t)0;   /* any even n contains the factor 2 → term 0 */

    seq_t result = (seq_t)1;
    idx_t rem = n;

    /* trial division for odd factors */
    for (idx_t p = 3; p <= rem / p; p += 2) {
        if (rem % p == 0) {
            idx_t exp = 0;
            while (rem % p == 0) {
                rem /= p;
                exp++;
            }

            /* term = (p+1)(p-2) */
            seq_t term = ((seq_t)p + (seq_t)1) * (((seq_t)p) - (seq_t)2);

            /* compute term^exp via binary exponentiation */
            seq_t pow_term = (seq_t)1;
            seq_t base = term;
            idx_t e = exp;
            while (e > 0) {
                if (e & 1) pow_term *= base;
                base *= base;
                e >>= 1;
            }

            result *= pow_term;
        }
    }

    /* remaining prime factor, if any */
    if (rem > 1) {
        seq_t term = ((seq_t)rem + (seq_t)1) * (((seq_t)rem) - (seq_t)2);
        result *= term;   /* exponent is 1 */
    }

    return result;
}
