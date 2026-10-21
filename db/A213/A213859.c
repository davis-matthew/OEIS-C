/*
 * A213859  a(n) = 2^n mod (n+2).
 * https://oeis.org/A213859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n mod (n+2)
 * Range:     u8 n <= 286, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute (n + 2) as the modulus */
    seq_t mod = (seq_t)n + (seq_t)2;

    /* Modular exponentiation of 2^n modulo mod */
    seq_t result = 1 % mod;          /* start value 1, but reduce by mod */
    seq_t base   = 2 % mod;          /* base of the exponentiation */
    idx_t exp    = n;                /* exponent n */

    while (exp) {
        if (exp & 1)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }

    return result;
}
