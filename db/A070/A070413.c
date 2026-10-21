/*
 * A070413  a(n) = 7^n mod 29.
 * https://oeis.org/A070413
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7^n mod 29
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t MOD = 29;
    seq_t result = 1 % MOD;   /* 7^0 mod 29 */
    seq_t base   = 7 % MOD;   /* 7^1 mod 29 */

    while (n)
    {
        if (n & 1ULL)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        n >>= 1;
    }
    return result;
}
