/*
 * A063518  Values of 17^n mod 23.
 * https://oeis.org/A063518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 17^n mod 23
 * Range:     u8 n <= 2000, u16 n <= 2000, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* fast modular exponentiation */
    const seq_t mod = 23;                 /* modulus */
    const seq_t base_init = 17 % mod;     /* base 17 modulo mod */
    seq_t result = 1;
    seq_t base = base_init;
    idx_t exp = n;

    while (exp > 0) {
        if (exp & 1) {
            result = (result * base) % mod;
        }
        base = (base * base) % mod;
        exp >>= 1;
    }
    return result;
}
