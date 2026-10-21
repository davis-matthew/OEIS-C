/*
 * A070376  a(n) = 5^n mod 26.
 * https://oeis.org/A070376
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5^n mod 26
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Since the modulus is very small we can perform modular exponentiation
     * with ordinary integer arithmetic while keeping all intermediate values
     * in the range of a 32‑bit unsigned integer. The final result fits in
     * seq_t because it is always < 26. */
    const unsigned int mod = 26u;
    unsigned int base  = 5u;   /* 5 mod 26 */
    unsigned int result = 1u;  /* 5^0 mod 26 */

    while (n)
    {
        if (n & 1u)
            result = (result * base) % mod;
        base = (base * base) % mod;
        n >>= 1;
    }
    return (seq_t)result;
}
