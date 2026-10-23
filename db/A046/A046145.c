/*
 * A046145  Smallest primitive root modulo n, or 0 if no root exists.
 * https://oeis.org/A046145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = smallest primitive root of n, or 0 if none
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul_mod64, ugcd_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard headers */
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

/* Utility: modular multiplication modulo 64-bit (no overflow) */
#include "../utils/mul_mod64.h"
#include "../utils/ugcd_uint64.h"

/* --------------------------------------------------------------------- */
/* Helper: Euclidean GCD for uint64_t ----------------------------------- */

/* Helper: modular exponentiation modulo 64-bit -------------------------- */
static uint64_t pow_mod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;          /* in case mod == 1 */
    base %= mod;
    while (exp) {
        if (exp & 1ULL)
            res = mul_mod64(res, base, mod);
        base = mul_mod64(base, base, mod);
        exp >>= 1ULL;
    }
    return res;
}

/* --------------------------------------------------------------------- */
/* Helper: factor a 64‑bit integer into distinct primes and exponents ---
   fac[i]   - i‑th distinct prime
   exp[i]   - multiplicity of fac[i]
   *cnt     - number of distinct primes found
   Max distinct primes for 64-bit < 16, so static arrays of size 16 are fine.
*/
static void factor_uint64(uint64_t n,
                          uint64_t fac[16], unsigned char exp[16], size_t *cnt)
{
    size_t c = 0;
    uint64_t t = n;

    for (uint64_t p = 2; p <= t / p; ++p) {
        if (t % p == 0) {
            fac[c] = p;
            exp[c] = 0;
            while (t % p == 0) {
                t /= p;
                ++exp[c];
            }
            ++c;
        }
    }
    if (t > 1) {
        fac[c]   = t;
        exp[c]   = 1;
        ++c;
    }
    *cnt = c;
}

/* --------------------------------------------------------------------- */
/* Main sequence function ------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Small cases */
    if (n == 0 || n == 1)
        return (seq_t)0;
    if (n == 2)
        return (seq_t)1;
    if (n == 4)
        return (seq_t)3;

    uint64_t N = (uint64_t)n;

    /* Factor N */
    uint64_t fac[16];
    unsigned char exp[16];
    size_t nc;
    factor_uint64(N, fac, exp, &nc);

    /* Determine if a primitive root exists */
    bool has_root = false;

    if (nc == 1) {
        /* n = p^k */
        if (fac[0] == 2) {
            /* 2^k with k <= 2 (handled above) -> no root */
            has_root = false;
        } else {
            /* odd prime power */
            has_root = true;
        }
    } else if (nc == 2) {
        /* n = 2 * p^k or p^k * 2 */
        if (fac[0] == 2 && exp[0] == 1 && fac[1] > 2) {
            has_root = true;                /* 2 * p^k */
        } else if (fac[1] == 2 && exp[1] == 1 && fac[0] > 2) {
            has_root = true;                /* p^k * 2 */
        } else {
            has_root = false;
        }
    } else {
        /* more than two distinct primes -> no primitive root */
        has_root = false;
    }

    if (!has_root)
        return (seq_t)0;

    /* Compute Euler's totient φ(N) */
    uint64_t phi = N;
    for (size_t i = 0; i < nc; ++i)
        phi -= phi / fac[i];

    /* Factor φ to obtain its distinct prime divisors */
    uint64_t phi_fac[16];
    unsigned char phi_exp[16];
    size_t phi_nc;
    factor_uint64(phi, phi_fac, phi_exp, &phi_nc);

    /* Search for the smallest primitive root */
    for (uint64_t g = 2; g < N; ++g) {
        if (ugcd_uint64(g, N) != 1)
            continue;                       /* must be coprime */

        /* Check that g has order φ(N) modulo N */
        if (pow_mod_uint64(g, phi, N) != 1)
            continue;                       /* not full order */

        bool ok = true;
        for (size_t i = 0; i < phi_nc; ++i) {
            uint64_t p = phi_fac[i];
            if (pow_mod_uint64(g, phi / p, N) == 1) {
                ok = false;                 /* order too small */
                break;
            }
        }
        if (ok)
            return (seq_t)g;                /* smallest found */
    }

    /* Should not reach here if has_root is true */
    return (seq_t)0;
}
