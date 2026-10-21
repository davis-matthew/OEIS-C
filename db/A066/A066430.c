/*
 * A066430  a(n) = 8^n mod n^8.
 * https://oeis.org/A066430
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 8^n mod n^8
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 16, u64 n <= 266, u128 n <= 500,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* multiply two 128‑bit unsigned numbers modulo mod; works for any
   mod that fits in 128 bits (true for n ≤ 10000) */
static unsigned __int128 mul_mod128(unsigned __int128 a, unsigned __int128 b, unsigned __int128 mod)
{
    unsigned __int128 res = 0;
    while (b)
    {
        if (b & 1)
        {
            res += a;
            if (res >= mod)
                res -= mod;
        }
        a = (a << 1) % mod;
        b >>= 1;
    }
    return res;
}

/* modular exponentiation  (base^exp mod mod) using mul_mod128 */
static unsigned __int128 pow_mod128(unsigned __int128 base, idx_t exp, unsigned __int128 mod)
{
    unsigned __int128 result = 1 % mod;
    while (exp)
    {
        if (exp & 1)
            result = mul_mod128(result, base, mod);
        base = mul_mod128(base, base, mod);
        exp >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* n^8 fits in 128 bits for n ≤ 10000 */
    unsigned __int128 mod = (unsigned __int128)n;
    for (idx_t i = 1; i < 8; ++i)
        mod *= (unsigned __int128)n;

    unsigned __int128 res = pow_mod128((unsigned __int128)8, n, mod);
    return (seq_t)res;
}
