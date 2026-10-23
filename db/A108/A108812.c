/*
 * A108812  11^n mod 50.
 * https://oeis.org/A108812
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^n mod 50
 * Range:     u8 n <= 74, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes 11^n modulo 50 using exponentiation by squaring. */
seq_t a(idx_t n)
{
    const seq_t mod = 50;          /* modulus */
    seq_t result = 1;              /* accumulate product */
    seq_t base   = 11;             /* starting base */

    /* Exponentiation by squaring: reduce n in binary. */
    while (n > 0) {
        if (n & 1)                /* if current bit is set, multiply */
            result = (result * base) % mod;
        base = (base * base) % mod;
        n >>= 1;
    }
    return result;
}
