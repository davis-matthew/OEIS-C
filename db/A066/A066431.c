/*
 * A066431  a(n) = 9^n mod n^9.
 * https://oeis.org/A066431
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 9^n mod n^9
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 12, u64 n <= 143, u128 n <= 500,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* 128‑bit arithmetic helper types */
typedef unsigned __int128 u128;

/* Addition modulo mod, safe against overflow (a,b < mod) */
static inline u128 add_mod128(u128 a, u128 b, u128 mod)
{
    u128 sum = a + b;
    /* if sum overflows or >= mod, reduce once */
    if (sum < a || sum >= mod)
        sum -= mod;
    return sum;
}

/* Multiply u128 'a' by small unsigned integer 'b' (< 2^32) modulo 'mod'.
   Uses addition to avoid 128→256 multiplication overflow. */
static inline u128 mul_mod_small(u128 a, uint64_t b, u128 mod)
{
    u128 result = 0;
    u128 base   = a % mod;
    while (b)
    {
        if (b & 1)
            result = add_mod128(result, base, mod);
        base = add_mod128(base, base, mod);
        b >>= 1;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Compute modulus M = n^9 as an arbitrary‑precision (128‑bit) value. */
    u128 M = 1;
    for (unsigned i = 0; i < 9; ++i)
        M *= (u128)n;

    /* Compute 9^n modulo M iteratively. */
    u128 res = 1 % M;
    for (idx_t i = 0; i < n; ++i)
        res = mul_mod_small(res, 9, M);

    /* Cast to the desired seq_t type.  If seq_t is 64‑bit,
       the cast will truncate appropriately; if it is 128‑bit,
       the value is preserved. */
    return (seq_t)res;
}
