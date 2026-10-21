/*
 * A065910  Second solution mod p of x^4 = 2 for primes p such that more than
 *          two solution exists.
 * https://oeis.org/A065910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   jacobi_symbol, mod_pow, next_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A065910: Second solution mod p of x^4 = 2 for primes p
   with more than two solutions (p ≡ 1 (mod 4) and 2 is a quartic residue).
   The function traverses primes, testing the condition until the n‑th
   qualifying prime is found, then returns the second smallest
   solution of x^4 ≡ 2 (mod p).  All arithmetic is performed in
   uint64_t; results are cast to the output type seq_t. */

/* Standard headers */
#include <stddef.h>
#include <stdint.h>

/* Utility headers (functions already implemented elsewhere) */
#include "../utils/mod_pow.h"          /* seq_t mod_pow(seq_t base, uint64_t exp, seq_t mod) */
#include "../utils/jacobi_symbol.h"   /* int jacobi_symbol(uint64_t a, uint64_t n) */
#include "../utils/next_prime.h"      /* uint64_t next_prime(uint64_t start) */

/*---------------------------------------------------------------------*/
/* fast modular multiplication  (a*b) mod mod, all values < 2^64 */
static inline uint64_t mul_mod_uint64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)(((__uint128_t)a * b) % mod);
}

/*---------------------------------------------------------------------*/
/* Tonelli–Shanks algorithm for square root modulo a prime p.
   Assumes that a is a quadratic residue mod p.               */
static uint64_t sqrt_mod_u64(uint64_t a, uint64_t p)
{
    if (p == 2) return a & 1ULL;

    /* If p ≡ 3 (mod 4), single exponentiation suffices */
    if (p % 4 == 3)
        return (uint64_t)mod_pow((seq_t)a, (p + 1) / 4, (seq_t)p);

    /* Factor p‑1 = q·2^s with q odd. */
    uint64_t q = p - 1;
    uint64_t s = 0;
    while ((q & 1ULL) == 0) { q >>= 1; ++s; }

    /* Find a quadratic non-residue z. */
    uint64_t z = 2;
    while (jacobi_symbol(z, p) != -1) ++z;

    seq_t c = mod_pow((seq_t)z, q, (seq_t)p);
    seq_t x = mod_pow((seq_t)a, (q + 1) / 2, (seq_t)p);
    seq_t t = mod_pow((seq_t)a, q, (seq_t)p);
    uint64_t m = s;

    while (t != 1) {
        /* Find the least i (0 < i < m) with t^(2^i) == 1. */
        uint64_t i = 1;
        seq_t t2 = mul_mod_uint64((uint64_t)t, (uint64_t)t, p);
        while (t2 != 1) {
            t2 = mul_mod_uint64((uint64_t)t2, (uint64_t)t2, p);
            ++i;
        }
        uint64_t e = m - i - 1;
        seq_t b = mod_pow(c, 1ULL << e, (seq_t)p);

        x = (seq_t)mul_mod_uint64((uint64_t)x, (uint64_t)b, p);
        t = (seq_t)mul_mod_uint64((uint64_t)t, (uint64_t)(mul_mod_uint64((uint64_t)b, (uint64_t)b, p)), p);
        c = (seq_t)mul_mod_uint64((uint64_t)b, (uint64_t)b, p);
        m = i;
    }
    return (uint64_t)x;
}

/*---------------------------------------------------------------------*/
/* Main sequence function                                           */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* undefined for n = 0 */

    idx_t good = 0;               /* number of qualifying primes seen */
    uint64_t p = 2;               /* current prime candidate (will be replaced) */

    while (true) {
        p = next_prime(p);
        if (p % 4 != 1) continue;                         /* must be 1 mod 4 */
        if (mod_pow((seq_t)2ULL, (p - 1) / 4, (seq_t)p) != 1) continue; /* quartic residue */

        ++good;
        if (good == n) {
            /* Compute the four solutions of x^4 ≡ 2 (mod p) */
            uint64_t r2 = sqrt_mod_u64(2, p);           /* sqrt(2)         */
            uint64_t s  = sqrt_mod_u64(r2, p);          /* fourth root     */
            uint64_t t  = sqrt_mod_u64(p - 1, p);       /* sqrt(-1)        */

            uint64_t a1 = s;
            uint64_t a2 = p - s;
            uint64_t a3 = mul_mod_uint64(s, t, p);
            uint64_t a4 = p - a3;

            /* Determine the second smallest among the four solutions */
            uint64_t vals[4] = {a1, a2, a3, a4};
            uint64_t min1 = UINT64_MAX, min2 = UINT64_MAX;
            for (int i = 0; i < 4; ++i) {
                uint64_t v = vals[i];
                if (v < min1) { min2 = min1; min1 = v; }
                else if (v < min2) { min2 = v; }
            }
            return (seq_t)min2;
        }
    }
}
