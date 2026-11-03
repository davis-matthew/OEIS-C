/*
 * A105853  a(n) = sigma(n) (mod 10), i.e., unit's digit of sigma(n).
 * https://oeis.org/A105853
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = sigma(n) mod 10 where sigma(n) is the sum of all positive
 *            divisors of n
 * Range:     u8 n <= 20000, u16 n <= 20000, u32 n <= 20000, u64 n <= 20000,
 *            u128 n <= 20000, big any n
 * Validated: standard, pass, 121 terms through n = 9215 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Compute sigma(n) modulo 10 using prime factorisation. */
static seq_t sigma_mod10(idx_t n)
{
    if (n == 1)  /* sigma(1) = 1 */
        return (seq_t)1;

    seq_t result = (seq_t)1;
    idx_t m = n;

    for (idx_t d = 2; d <= m / d; ++d)
    {
        if (m % d == 0)
        {
            /* count exponent of prime d in n */
            int exp = 0;
            while (m % d == 0) {
                m /= d;
                ++exp;
            }

            /* sum of powers: 1 + d + d^2 + ... + d^exp (mod 10) */
            seq_t sum   = (seq_t)1;          /* d^0 */
            seq_t power = (seq_t)1;          /* current d^k mod 10 */

            for (int k = 0; k < exp; ++k) {
                power = (power * (d % 10)) % 10;   /* d^(k+1) */
                sum   = (sum + power) % 10;
            }

            result = (result * sum) % 10;
        }
    }

    /* Remaining prime factor (if any) */
    if (m > 1) {
        seq_t pmod = m % 10;               /* m is prime */
        seq_t sum  = (seq_t)((1 + pmod) % 10);
        result = (result * sum) % 10;
    }

    return result;
}

/* External interface required by the framework. */
seq_t a(idx_t n)
{
    return sigma_mod10(n);
}
