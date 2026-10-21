/*
 * A201920  a(n) = 2^n mod 125.
 * https://oeis.org/A201920
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n mod 125
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Fast exponentiation by squaring, modulo 125. */
    seq_t result = 1;
    seq_t base   = 2;
    idx_t exp   = n;

    const seq_t MOD = (seq_t)125;

    while (exp != 0) {
        if (exp & 1)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1;
    }
    return result;
}
