/*
 * A082494  a(n) = n - (2^n (mod n)).
 * https://oeis.org/A082494
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, look
 * Formula:   a(n) = n - (2^n mod n)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

static inline uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 0)
        return 0;               /* not used, but keeps the function robust */
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp)
    {
        if (exp & 1)
            result = (uint64_t)(((__uint128_t)result * b) % mod);
        b = (uint64_t)(((__uint128_t)b * b) % mod);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    uint64_t mod = mod_pow_uint64(2, n, n);
    return (seq_t)n - (seq_t)mod;
}
