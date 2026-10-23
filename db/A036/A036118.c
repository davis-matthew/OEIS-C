/*
 * A036118  a(n) = 2^n mod 13.
 * https://oeis.org/A036118
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n mod 13
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)2;
    const seq_t mod = (seq_t)13;

    while (n > 0)
    {
        if (n & 1u)
            result = (result * base) % mod;
        base = (base * base) % mod;
        n >>= 1;
    }
    return result;
}
