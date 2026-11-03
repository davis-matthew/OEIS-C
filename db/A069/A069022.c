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
 * Validated: partial, pass, 21 terms through n = 21 (2026-11-01)
 * Helpers:   ipow_idx, powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/ipow_idx.h"
#include "../utils/powmod.h"

/* Inverse of base modulo primeⁿ, where prime is 2 or 3 and base is 2 or 3. */
static seq_t modinv_prime_power(seq_t base, idx_t n, uint64_t prime)
{
    /* initial inverse modulo the prime */
    seq_t inv;
    if (prime == 2)                          /* 3 mod 2 = 1, inverse is 1 */
        inv = 1;
    else if (prime == 3) {                  /* 2 mod 3 = 2, 2⁻¹ ≡ 2 (mod 3) */
        inv = 2;
    } else {
        /* generic (not used in this sequence) */
        seq_t bmod = base % (seq_t)prime;
        for (seq_t i = 1; i < (seq_t)prime; ++i) {
            if ((bmod * i) % (seq_t)prime == 1) { inv = i; break; }
        }
    }

    if (n == 1) return inv;

    seq_t mod = (seq_t)prime;                /* current power prime¹ */
    for (idx_t exp = 2; exp <= n; ++exp) {
        seq_t next = mod * (seq_t)prime;     /* primeᵉᵠᵖ */
        seq_t prod = (base * inv) % next;
        seq_t temp;
        if (prod >= 2)
            temp = next - (prod - (seq_t)2);
        else
            temp = (seq_t)2 - prod;
        inv = (inv * temp) % next;
        mod = next;
    }
    return inv;
}

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    seq_t two = (seq_t)2;
    seq_t three = (seq_t)3;

    seq_t powA = ipow_idx(two, n);   /* 2ⁿ */
    seq_t powB = ipow_idx(three, n); /* 3ⁿ */

    /* First candidate: m ≡ 0 (mod 2ⁿ), m+1 ≡ 0 (mod 3ⁿ) */
    seq_t inv2mod3n = modinv_prime_power(two, n, 3);          /* inverse of 2 mod 3ⁿ */
    seq_t invAmodB   = powmod(inv2mod3n, n, powB);           /* inverse of 2ⁿ mod 3ⁿ */
    seq_t t12        = (powB - invAmodB) % powB;              /* minimal positive k */
    seq_t m12        = powA * t12;

    /* Second candidate: m ≡ 0 (mod 3ⁿ), m+1 ≡ 0 (mod 2ⁿ) */
    seq_t inv3mod2n = modinv_prime_power(three, n, 2);        /* inverse of 3 mod 2ⁿ */
    seq_t invBmodA   = powmod(inv3mod2n, n, powA);           /* inverse of 3ⁿ mod 2ⁿ */
    seq_t t21        = (powA - invBmodA) % powA;
    seq_t m21        = powB * t21;

    return (m12 < m21) ? m12 : m21;
}
