/*
 * A070369  a(n) = 5^n mod 14.
 * https://oeis.org/A070369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n mod 14
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base   = 5 % 14;          /* reduce base modulo 14 */
    idx_t exp    = n;               /* exponent (may be large) */

    while (exp > 0) {
        if (exp & 1) {
            result = (result * base) % 14;
        }
        base = (base * base) % 14;
        exp >>= 1;
    }
    return result;
}
