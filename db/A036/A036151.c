/*
 * A036151  5^n mod 167.
 * https://oeis.org/A036151
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n mod 167
 * Range:     u8 n <= 47, u16 n <= 47, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Use unsigned 64-bit arithmetic for modular exponentiation.
       The modulus 167 easily fits in 64 bits, and intermediate
       products stay below 2^64, so no overflow occurs. */
    unsigned long long result = 1ULL;
    unsigned long long base   = 5ULL;
    const unsigned long long mod = 167ULL;
    idx_t exp = n;

    while (exp)
    {
        if (exp & 1ULL)
            result = (result * base) % mod;
        base = (base * base) % mod;
        exp >>= 1;
    }
    return (seq_t)result;
}
