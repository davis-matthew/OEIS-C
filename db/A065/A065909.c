/*
 * A065909  First solution mod p of x^4 = 2 for primes p such that more than
 *          two solutions exist.
 * https://oeis.org/A065909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 18, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime, mul64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS sequence A065909.
   For n ≥ 1: a(n) is the least integer x (0 ≤ x < p) such that
   x⁴ ≡ 2 (mod p), where p is the n‑th prime for which x⁴ = 2 has
   four solutions modulo p (i.e. 2 is a biquadratic residue mod p).
   The implementation searches primes sequentially, tests the
   biquadratic condition by modular exponentiation, computes the
   four fourth‑roots via Tonelli–Shanks square‑root algorithm,
   and caches results for future calls. */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"   /* is_prime(idx_t) -> bool              */
#include "../utils/mul64.h"      /* mul64(uint64_t, uint64_t, uint64_t) */

static inline uint64_t mod_mul64(uint64_t a, uint64_t b, uint64_t mod)
{
    return mul64(a, b, mod);   /* safe for 64‑bit operands            */
}

static inline uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t r = 1 % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1) r = mod_mul64(r, base, mod);
        base = mod_mul64(base, base, mod);
        exp >>= 1;
    }
    return r;
}

/* Legendre symbol (a|p), p odd prime. */
static inline int legendre_symbol(uint64_t a, uint64_t p)
{
    uint64_t ls = mod_pow64(a, (p - 1) >> 1, p);
    if (ls == p - 1) return -1;
    return (int)ls;          /* 0 or 1                                 */
}

/* Tonelli–Shanks: modular square root mod odd prime p.
   Returns 0 if no square root exists.            */
static uint64_t sqrt_mod(uint64_t a, uint64_t p)
{
    if (a % p == 0) return 0;
    if (p == 2) return a & 1;
    if (legendre_symbol(a, p) == -1) return 0;  /* non‑residue */

    uint64_t q = p - 1;
    uint64_t s = 0;
    while ((q & 1) == 0)
    {
        q >>= 1;
        ++s;
    }

    if (s == 1)
        return mod_pow64(a, (p + 1) >> 2, p);

    /* find quadratic non‑residue z */
    uint64_t z = 2;
    while (legendre_symbol(z, p) != -1) ++z;

    uint64_t c = mod_pow64(z, q, p);
    uint64_t t = mod_pow64(a, q, p);
    uint64_t r = mod_pow64(a, (q + 1) >> 1, p);
    uint64_t m = s;

    while (t != 1)
    {
        uint64_t tt = t;
        uint64_t i = 0;
        for (i = 1; i < m; ++i)
        {
            tt = mod_mul64(tt, tt, p);
            if (tt == 1) break;
        }
        uint64_t b = mod_pow64(c, 1ULL << (m - i - 1), p);
        r = mod_mul64(r, b, p);
        c = mod_mul64(b, b, p);
        t = mod_mul64(t, c, p);
        m = i;
    }
    return r;
}

/* Main sequence function ------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < 1) return 0;

    /* cache for previously computed terms */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* smallest odd candidate prime to test */
    static uint64_t next_candidate = 3;

    /* Extend cache until we have a(n) */
    while (cache_len < n)
    {
        /* Find next qualifying prime */
        for (;;)
        {
            if (!is_prime(next_candidate))
            {
                next_candidate += 2;
                continue;
            }

            uint64_t p = next_candidate;

            /* 2 must be a fourth‑power residue: 2^((p‑1)/4) ≡ 1 (mod p) */
            if (mod_pow64(2, (p - 1) >> 2, p) != 1)
            {
                next_candidate += 2;
                continue;
            }

            /* Compute the four fourth‑roots of 2 modulo p */
            uint64_t r1 = sqrt_mod(2, p);      /* sqrt(2)          */
            if (r1 == 0) { next_candidate += 2; continue; }
            uint64_t r2 = sqrt_mod(r1, p);     /* sqrt(sqrt(2))    */
            if (r2 == 0) { next_candidate += 2; continue; }
            uint64_t g  = sqrt_mod(p - 1, p);  /* sqrt(-1)         */
            if (g == 0) { next_candidate += 2; continue; }

            uint64_t a0 = r2;
            uint64_t a1 = p - r2;
            uint64_t a2 = mod_mul64(r2, g, p);
            uint64_t a3 = p - a2;

            uint64_t min = a0;
            if (a1 < min) min = a1;
            if (a2 < min) min = a2;
            if (a3 < min) min = a3;

            /* Store in cache */
            if (cache_len == cache_cap)
            {
                cache_cap = (cache_cap == 0) ? 64 : cache_cap << 1;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = (seq_t)min;

            /* Advance candidate beyond current prime for next search */
            next_candidate = p + 2;
            break;     /* found next term, go back to outer loop */
        }
    }

    return cache[n - 1];
}
