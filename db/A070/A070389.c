/*
 * A070389  a(n) = 5^n mod 43.
 * https://oeis.org/A070389
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n mod 43
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const idx_t period = 42;          /* multiplicative order of 5 modulo 43 */
    idx_t k = n % period;             /* reduce exponent modulo the period */

    seq_t result = 1;
    seq_t base   = 5;
    const seq_t mod = 43;

    /* binary exponentiation */
    while (k > 0) {
        if (k & 1)
            result = (result * base) % mod;
        k >>= 1;
        if (k)                 /* avoid unnecessary squaring on last iteration */
            base = (base * base) % mod;
    }
    return result;
}
