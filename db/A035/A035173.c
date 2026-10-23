/*
 * A035173  Coefficients in expansion of Dirichlet series Product_p
 *          (1-(Kronecker(m,p)+1)*p^(-s)+Kronecker(m,p)*p^(-2s))^(-1) for m =
 *          -17.
 * https://oeis.org/A035173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, mult
 * Formula:   a(n) = Sum_{d|n} Kronecker(-17, d) (OEIS A035173)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* 64‑bit modular multiplication using 128‑bit intermediate */
static inline uint64_t mod_mul64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

/* 64‑bit modular exponentiation */
static inline uint64_t mod_pow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1ULL)
            result = mod_mul64(result, base, mod);
        base = mod_mul64(base, base, mod);
        exp >>= 1ULL;
    }
    return result;
}

/* Kronecker symbol (‑17 / p) for a prime p.
   p == 2 returns 1, p == 17 returns 0, otherwise μ = ±1. */
static inline int kronecker_neg17_prime(uint64_t p)
{
    if (p == 2) return 1;      /* (-17/2) = 1 */
    if (p == 17) return 0;     /* divides the argument */

    /* a = (-17) mod p  (never zero here because p≠17) */
    uint64_t a = (p - (17 % p)) % p;
    uint64_t leg = mod_pow64(a, (p - 1) / 2, p);
    /* leg == 1 → +1, leg == p-1 → -1 (Legendre symbol) */
    return (leg == 1ULL) ? 1 : -1;
}

/* Sequence a(n) for A035173. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;       /* sequence defined for n≥1 only */

    uint64_t nn = (uint64_t)n;
    seq_t result = 1;

    /* factor 2 */
    unsigned exp = 0;
    while ((nn & 1ULL) == 0ULL)
    {
        ++exp;
        nn >>= 1;
    }
    if (exp > 0)                   /* Kronecker(-17,2) = 1 */
        result *= (seq_t)(exp + 1);

    /* odd prime factors */
    for (uint64_t p = 3; p * p <= nn; p += 2)
    {
        if (nn % p == 0)
        {
            exp = 0;
            while (nn % p == 0)
            {
                ++exp;
                nn /= p;
            }

            int sym = kronecker_neg17_prime(p);
            if (sym == -1)
            {
                if (exp & 1) return 0;   /* odd exponent → zero */
                /* even exponent → contributes 1, nothing to do */
            }
            else /* sym == 1 */
            {
                result *= (seq_t)(exp + 1);
            }
        }
    }

    /* remaining prime factor > sqrt(original n) */
    if (nn > 1)
    {
        uint64_t p = nn;
        if (p != 17)               /* special case p = 17 → coefficient 1 */
        {
            int sym = kronecker_neg17_prime(p);
            if (sym == -1) return 0;   /* exponent 1 is odd */
            result *= (seq_t)(2);       /* exp = 1 → multiply by (1+1) */
        }
    }

    return result;
}
