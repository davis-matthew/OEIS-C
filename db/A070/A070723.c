/*
 * A070723  n^7 mod 43.
 * https://oeis.org/A070723
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^7 mod 43
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* constant modulus */
    const seq_t MOD = (seq_t)43;
    seq_t result = (seq_t)1;
    seq_t base   = (seq_t)n % MOD;
    uint8_t exp = 7;   /* fixed exponent */

    /* fast exponentiation – O(log exp) = O(1) for exp = 7 */
    while (exp)
    {
        if (exp & 1U)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1U;
    }
    return result;
}
