/*
 * A167351  Totally multiplicative sequence with a(p) = (p+1)*(p+2) = p^2+3p+2
 *          for prime p.
 * https://oeis.org/A167351
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = ∏_{p^e‖n} ((p+1)(p+2))^e (multiplicative with a(p) = p^2
 *            + 3p + 2)
 * Range:     u8 n <= 7, u16 n <= 31, u32 n <= 511, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;   /* a(1) = 1 */

    seq_t result = 1;
    idx_t remaining = n;

    /* Trial division factorisation; n ≤ 10 000 in typical uses,
       so this is fast enough and requires no external helpers. */
    for (idx_t p = 2; p * p <= remaining; ++p) {
        if (remaining % p == 0) {
            idx_t exp = 0;
            while (remaining % p == 0) {
                remaining /= p;
                ++exp;
            }

            /* base = (p+1)(p+2)  */
            seq_t base = (seq_t)(p + 1) * (p + 2);

            /* multiply result by base^exp */
            seq_t power = 1;
            for (idx_t i = 0; i < exp; ++i)
                power = power * base;

            result = result * power;
        }
    }

    if (remaining > 1) {           /* remaining is a prime factor with exp = 1 */
        seq_t base = (seq_t)(remaining + 1) * (remaining + 2);
        result = result * base;
    }

    return result;
}
