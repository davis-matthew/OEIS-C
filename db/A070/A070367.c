/*
 * A070367  a(n) = 5^n mod 11.
 * https://oeis.org/A070367
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n mod 11
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;           /* accumulating result modulo 11 */
    seq_t base   = 5;           /* current power of 5 modulo 11 */
    seq_t mod    = 11;          /* modulus */

    idx_t e = n;                /* exponent as index type */
    while (e > 0) {
        if (e & 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        e >>= 1;
    }
    return result;
}
