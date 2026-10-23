/*
 * A036156  5^n mod 193.
 * https://oeis.org/A036156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n mod 193
 * Range:     u8 n <= 48, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t result = 1;
    seq_t base   = 5;
    seq_t mod    = 193;
    while (n > 0)
    {
        if (n & 1ULL)
            result = (result * base) % mod;
        base = (base * base) % mod;
        n >>= 1;
    }
    return result;
}
