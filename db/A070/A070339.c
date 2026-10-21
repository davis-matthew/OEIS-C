/*
 * A070339  a(n) = 2^n mod 35.
 * https://oeis.org/A070339
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n mod 35
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t MOD = 35;
    seq_t result = 1 % MOD;
    seq_t base   = 2 % MOD;
    while (n > 0)
    {
        if (n & 1U)
            result = (seq_t)((result * base) % MOD);
        base = (seq_t)((base * base) % MOD);
        n >>= 1;
    }
    return result;
}
