/*
 * A070348  a(n) = 2^n mod 41.
 * https://oeis.org/A070348
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n mod 41
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    const seq_t MOD = (seq_t)41;   /* the modulus */
    seq_t result = (seq_t)1;      /* accumulating result */
    seq_t base   = (seq_t)2;      /* current power of two */
    idx_t exp = n;                /* exponent in idx_t form */

    /* binary exponentiation */
    while (exp > 0)
    {
        if (exp & 1ULL)
            result = (result * base) % MOD;
        base = (base * base) % MOD;
        exp >>= 1ULL;
    }
    return result;
}
