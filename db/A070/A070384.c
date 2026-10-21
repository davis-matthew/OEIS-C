/*
 * A070384  a(n) = 5^n mod 37.
 * https://oeis.org/A070384
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 5^n mod 37
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t modulus = 37;          /* modulus */
    const seq_t base = 5;              /* base of the exponentiation */
    /* Fast exponentiation (binary exponentiation) with exponent reduced modulo 36
       (Euler's theorem: 5^36 ≡ 1 (mod 37)).  Handles any n ≥ 0. */
    idx_t exp = n % 36;                /* reduce exponent by the period */
    seq_t result = 1;
    seq_t cur = base;
    while (exp > 0) {
        if (exp & 1) result = (result * cur) % modulus;
        cur = (cur * cur) % modulus;
        exp >>= 1;
    }
    return result;
}
