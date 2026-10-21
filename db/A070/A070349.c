/*
 * A070349  a(n) = 2^n mod 43.
 * https://oeis.org/A070349
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n mod 43
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* 43 is small enough that intermediate products stay within 16 bits */
    const seq_t MOD = (seq_t)43;
    seq_t result = (seq_t)1;      /* 2^0 mod 43 */
    seq_t base   = (seq_t)2;      /* 2^1 mod 43 */

    /* Use binary exponentiation */
    idx_t exp = n;
    while (exp != 0)
    {
        if (exp & (idx_t)1)
            result = (result * base) % MOD;    /* multiply and reduce */
        base = (base * base) % MOD;            /* square and reduce */
        exp >>= 1;
    }
    return result;
}
