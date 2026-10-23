/*
 * A036130  a(n) = 2^n mod 61.
 * https://oeis.org/A036130
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n mod 61
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t mod   = 61;      /* modulus */
    seq_t result = 1;      /* accumulator */
    seq_t base   = 2;      /* current power of 2 (mod mod) */
    idx_t exp = n;         /* exponent, fits in idx_t */

    while (exp) {
        if (exp & 1u)
            result = (result * base) % mod;
        exp >>= 1;
        base = (base * base) % mod;
    }
    return result;
}
