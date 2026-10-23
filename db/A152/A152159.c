/*
 * A152159  A sequence set up on the base 16 Pi digits BBP:
 *          a(n)=Floor[Mod[(4/(8*n + 1) - 2/(8*n + 4) - 1/(8*n + 5) - 1/(8*n +
 *          6))*16^n, 4]].
 * https://oeis.org/A152159
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Floor[Mod[(4/(8*n + 1) - 2/(8*n + 4) - 1/(8*n + 5) -
 *            1/(8*n + 6))*16^n, 4]]
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* 128‑bit integer types (built‑in) */
typedef unsigned __int128 u128;
typedef __int128          i128;

/* Euclidean algorithm for 128‑bit unsigned integers */
static u128 gcd128(u128 a, u128 b)
{
    while (b != 0) {
        u128 t = a % b;
        a = b;
        b = t;
    }
    return a;
}

/* Add a fraction term (term_num/term_den) to the current fraction
   represented by (num/den).  All arithmetic is performed in 128‑bit
   integers and the fraction is reduced immediately. */
static void add_frac(i128 *num, u128 *den, i128 term_num, u128 term_den)
{
    i128 new_num = (*num) * (i128)term_den + term_num * (i128)(*den);
    u128  new_den = (*den) * term_den;

    u128 abs_num = new_num >= 0 ? (u128)new_num : (u128)(-new_num);
    u128 g = gcd128(abs_num, new_den);

    *num = new_num / (i128)g;
    *den = new_den / g;
}

/* Multiply two 128‑bit unsigned integers modulo mod without overflowing.
   This uses the standard "Russian peasant" algorithm. */
static u128 mul_mod(u128 a, u128 b, u128 mod)
{
    u128 res = 0;
    a %= mod;
    while (b) {
        if (b & 1u)
            res = (res + a) % mod;
        a = (a << 1u) % mod;
        b >>= 1u;
    }
    return res;
}

/* Compute base^exp mod mod for 128‑bit unsigned integers. */
static u128 powmod(u128 base, idx_t exp, u128 mod)
{
    u128 result = 1 % mod;
    while (exp) {
        if (exp & 1u)
            result = mul_mod(result, base, mod);
        base = mul_mod(base, base, mod);
        exp >>= 1u;
    }
    return result;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Compute the rational sum */
    i128 num = (i128)4;
    u128 den = (u128)(8 * n + 1);

    add_frac(&num, &den, (i128)-2, (u128)(8 * n + 4));
    add_frac(&num, &den, (i128)-1, (u128)(8 * n + 5));
    add_frac(&num, &den, (i128)-1, (u128)(8 * n + 6));

    /* Avoid negative num: the final result must be non‑negative */
    u128 num_mod = num >= 0 ? (u128)num : (u128)(-num);
    u128 den_val = den;

    u128 modDen = den_val * 4u;          /* modulus for the floor operation */

    /* 16^n mod (4*den) */
    u128 pow16 = powmod((u128)16, n, modDen);

    /* (num mod modDen) * 16^n mod modDen */
    u128 prod = mul_mod(num_mod % modDen, pow16, modDen);

    /* The final value is floor(prod / den) */
    seq_t result = (seq_t)(prod / den_val);

    return result;
}
