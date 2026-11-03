/*
 * A091025  Smallest positive k such that phi(1+k*2^m) <= phi(k*2^m) for all m
 *          = n (mod 12), where phi is Euler's totient function.
 * https://oeis.org/A091025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, fini, full
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 11, u32 n <= 11, u64 n <= 11, u128 n <= 11,
 *            big any n
 * Validated: full, pass, 12 terms through n = 11 (2026-11-02)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   A091025:  a(n) is the smallest positive integer k such that
   φ(1 + k·2^m) ≤ φ(k·2^m) for all m ≡ n (mod 12).
   This holds iff 1 + k·2^m has prime factors 3, 5 and 7 for all
   such m.  Hence k must satisfy
        k·2^n ≡ -1 (mod p)   for p = 3, 5, 7.
   The solution for k is unique modulo 105 = 3·5·7, and the
   minimal positive residue is the desired term a(n).
   The value depends only on n mod 12, giving a period of 12.
*/

#include <stdint.h>

/* small modular exponentiation, result < 1000 */
static uint64_t modpow2(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    base %= mod;
    while (exp) {
        if (exp & 1ULL) res = (res * base) % mod;
        base = (base * base) % mod;
        exp >>= 1ULL;
    }
    return res;
}

/* Fermat inverse for a prime modulus p (< uint64_t) */
static uint64_t modinv_small(uint64_t a, uint64_t p)
{
    /* a^(p-2) mod p */
    return modpow2(a, p - 2ULL, p);
}

/*
   Combine congruences r_i mod m_i (m_i prime) using Garner's algorithm.
   The moduli are 3, 5, 7, product 105.
*/
static uint64_t crt_3_5_7(uint64_t r3, uint64_t r5, uint64_t r7)
{
    uint64_t m1 = 3, m2 = 5, m3 = 7;
    uint64_t x, inv, t;
    uint64_t res = 0;
    uint64_t prod = 1;

    /* first modulus 3 */
    res = r3;
    prod = m1;

    /* second modulus 5 */
    inv = modinv_small(prod % m2, m2);
    t   = (r5 + m2 - res % m2) % m2;
    t   = (t * inv) % m2;
    res += t * prod;
    prod *= m2;

    /* third modulus 7 */
    inv = modinv_small(prod % m3, m3);
    t   = (r7 + m3 - res % m3) % m3;
    t   = (t * inv) % m3;
    res += t * prod;
    prod *= m3;

    return res % prod;           /* product = 105 */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* reduce exponent modulo the order of 2 modulo each prime:
       order(2 mod 3) = 2, order(2 mod 5) = 4, order(2 mod 7) = 3 */
    uint64_t exp3 = n % 2ULL;
    uint64_t exp5 = n % 4ULL;
    uint64_t exp7 = n % 3ULL;

    /* compute 2^n mod each prime */
    uint64_t pow2_3 = modpow2(2ULL, exp3, 3ULL);
    uint64_t pow2_5 = modpow2(2ULL, exp5, 5ULL);
    uint64_t pow2_7 = modpow2(2ULL, exp7, 7ULL);

    /* k ≡ - (2^n)^{-1} (mod p) */
    uint64_t r3 = (3ULL - modinv_small(pow2_3, 3ULL)) % 3ULL;
    uint64_t r5 = (5ULL - modinv_small(pow2_5, 5ULL)) % 5ULL;
    uint64_t r7 = (7ULL - modinv_small(pow2_7, 7ULL)) % 7ULL;

    /* combine residues via CRT */
    uint64_t k = crt_3_5_7(r3, r5, r7);   /* 1 ≤ k ≤ 104 */

    return (seq_t)k;
}
