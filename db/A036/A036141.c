/*
 * A036141  a(n) = 6^n mod 109.
 * https://oeis.org/A036141
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6^n mod 109
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t MOD = (seq_t)109;
    seq_t result = 1;
    seq_t base   = 6 % MOD;

    while (n > 0)
    {
        if (n & 1)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        n >>= 1;
    }
    return result;
}
