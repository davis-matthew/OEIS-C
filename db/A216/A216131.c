/*
 * A216131  a(n) = 11^n mod 1000.
 * https://oeis.org/A216131
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 11^n mod 1000
 * Range:     u8 n <= 2, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t MOD = 1000;   /* modulus */
    seq_t result = 1;
    seq_t base   = 11;
    while (n)
    {
        if (n & 1)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        n >>= 1;
    }
    return result;
}
