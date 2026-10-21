/*
 * A069022  Start of the first occurrence of two consecutive numbers divisible
 *          by an n-th power.
 * https://oeis.org/A069022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   for n>=1, a(n) = the minimal m such that ∃a,b≥2 with aⁿ|m and
 *            bⁿ|(m+1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   mod_pow, modinv_prime_power, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"
#include "../utils/mod_pow.h"
#include "../utils/modinv_prime_power.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    seq_t two = (seq_t)2;
    seq_t three = (seq_t)3;

    seq_t powA = seq_pow(two, n);   /* 2ⁿ */
    seq_t powB = seq_pow(three, n); /* 3ⁿ */

    /* First candidate: m ≡ 0 (mod 2ⁿ), m+1 ≡ 0 (mod 3ⁿ) */
    seq_t inv2mod3n = modinv_prime_power(two, n, 3);          /* inverse of 2 mod 3ⁿ */
    seq_t invAmodB   = mod_pow(inv2mod3n, n, powB);           /* inverse of 2ⁿ mod 3ⁿ */
    seq_t t12        = (powB - invAmodB) % powB;              /* minimal positive k */
    seq_t m12        = powA * t12;

    /* Second candidate: m ≡ 0 (mod 3ⁿ), m+1 ≡ 0 (mod 2ⁿ) */
    seq_t inv3mod2n = modinv_prime_power(three, n, 2);        /* inverse of 3 mod 2ⁿ */
    seq_t invBmodA   = mod_pow(inv3mod2n, n, powA);           /* inverse of 3ⁿ mod 2ⁿ */
    seq_t t21        = (powA - invBmodA) % powA;
    seq_t m21        = powB * t21;

    return (m12 < m21) ? m12 : m21;
}
