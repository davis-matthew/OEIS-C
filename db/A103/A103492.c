/*
 * A103492  Multiplicative suborder of 6 (mod 2n+1) = sord(6, 2n+1).
 * https://oeis.org/A103492
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 if 2n+1 is divisible by 3; otherwise a(n) = smallest e
 *            ≥ 1 with 6^e ≡ ±1 (mod 2n+1). The code computes the
 *            multiplicative order of 6 mod (2n+1) and then, if that order is
 *            even and 6^(order/2) ≡ -1 mod (2n+1), returns order/2, else
 *            returns the full order. It uses a simple trial‑division f...
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   factor_primes_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/factor_primes_u64.h"

/* ----- helper: power modulo 64‑bit ---------------------------------------
 * Uses binary exponentiation, all arithmetic performed modulo 'mod'.
 * Note: caller must ensure that 'mod' fits in 64 bits.
 */
static uint64_t modpow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1;
    base %= mod;
    while (exp > 0) {
        if (exp & 1ULL) result = (uint64_t)((__uint128_t)result * base % mod);
        base = (uint64_t)((__uint128_t)base * base % mod);
        exp >>= 1;
    }
    return result;
}

/* ----- main sequence ------------------------------------------------------
 * Computes a(n) for n ≥ 0.  The function works with arbitrary index
 * values, but keep in mind that for very large n the multiplication
 * 2*n+1 must still fit in 64 bits; otherwise the behaviour is undefined.
 */
seq_t a(idx_t n)
{
    /* special case n==0: by definition a(0)=0 */
    if (n == 0) return (seq_t)0;

    /* modulus m = 2n+1 (must fit in 64 bits) */
    uint64_t m = 2ULL * (uint64_t)n + 1ULL;

    /* if 3 divides m, no exponent satisfies 6^e ≡ ±1 (mod m) */
    if (m % 3ULL == 0ULL) return (seq_t)0;

    /* factor m to compute φ(m) */
    uint64_t prime_m[32];
    size_t cnt_m;
    factor_primes_u64(m, prime_m, &cnt_m);

    /* compute φ(m) = m * Π (1 - 1/p) */
    uint64_t phi = m;
    for (size_t i = 0; i < cnt_m; ++i) {
        uint64_t p = prime_m[i];
        phi = phi / p * (p - 1ULL);
    }

    /* factor φ(m) to obtain its prime divisors */
    uint64_t prime_phi[32];
    size_t cnt_phi;
    factor_primes_u64(phi, prime_phi, &cnt_phi);

    /* compute multiplicative order of 6 modulo m */
    uint64_t order = phi;
    for (size_t i = 0; i < cnt_phi; ++i) {
        uint64_t p = prime_phi[i];
        while (order % p == 0ULL) {
            uint64_t cand = order / p;
            if (modpow_uint64(6ULL, cand, m) == 1ULL)
                order = cand;
            else
                break;
        }
    }

    /* check for a smaller exponent giving –1 */
    if ((order & 1ULL) == 0ULL) {
        uint64_t half = order >> 1;
        if (modpow_uint64(6ULL, half, m) == (m - 1ULL) % m)
            return (seq_t)half;
    }

    return (seq_t)order;
}
