/*
 * A187096  Coefficients of L-series for elliptic curve "19a3": y^2 + y = x^3
 *          + x^2 + x.
 * https://oeis.org/A187096
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, mult
 * Formula:   a(n) is multiplicative. * For a prime power pᵉ (p ≠ 19) we have
 *            * a(p) = p – N(p) where N(p) = #points of y² + y = x³ + x² + x
 *            over 𝔽ₚ * a(pᵉ) = a(p)·a(p^{e‑1}) – p·a(p^{e‑2}) (e ≥ 2) * For p
 *            = 19 we set a(19ᵉ) = 1 for every e ≥ 0. * a(1) = 1. Offset = 1
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/*==========================================================
 *  Local helpers: modular arithmetic for 64‑bit numbers
 *==========================================================*/
static inline uint64_t mul_mod64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (uint64_t)((__uint128_t)a * b % mod);
}

static inline uint64_t pow_mod64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t res = 1 % mod;
    while (exp)
    {
        if (exp & 1) res = mul_mod64(res, base, mod);
        base = mul_mod64(base, base, mod);
        exp >>= 1;
    }
    return res;
}

/*==========================================================
 *  Count affine points of the elliptic curve mod p and
 *  return the trace a(p) = p – N_affine
 *==========================================================*/
static seq_t elliptic_trace(uint64_t p)
{
    if (p == 2)          /* special case: enumeration is simpler */
    {
        seq_t N = 0;
        for (uint64_t x = 0; x < 2; ++x)
        {
            uint64_t rhs = (x * x * x + x * x + x) & 1ULL; /* mod 2 */
            if (rhs == 0) N += 2;      /* (0,0),(0,1) for rhs=0 */
        }
        return (seq_t)p - N;          /* a(2) = 0 */
    }

    uint64_t N = 0;                   /* affine points count */
    for (uint64_t x = 0; x < p; ++x)
    {
        uint64_t x2 = mul_mod64(x, x, p);
        uint64_t x3 = mul_mod64(x2, x, p);

        uint64_t rhs = x3;
        rhs = (rhs + x2) % p;
        rhs = (rhs + x) % p;

        /* discriminant D = 1 + 4*rhs  (mod p) */
        uint64_t D = mul_mod64(4, rhs, p);
        D = (D + 1) % p;

        if (D == 0)            /* one solution */
            N += 1;
        else
        {
            uint64_t leg = pow_mod64(D, (p - 1) / 2, p);
            if (leg == 1)      /* two solutions */
                N += 2;
        }
    }
    return (seq_t)p - (seq_t)N;
}

/*==========================================================
 *  Compute a(p^e) using the recurrence or the special rule
 *==========================================================*/
static seq_t prime_power_value(uint64_t p, unsigned exp)
{
    if (exp == 0) return (seq_t)1;
    if (p == 19)   return (seq_t)1;     /* a(19^e) = 1 for all e */

    seq_t ap = elliptic_trace(p);

    if (exp == 1) return ap;

    seq_t prev  = ap;   /* a(p)       */
    seq_t prev2 = (seq_t)1;          /* a(p^0) = 1 */
    for (unsigned i = 2; i <= exp; ++i)
    {
        seq_t cur = ap * prev - (seq_t)p * prev2;
        prev2 = prev;
        prev  = cur;
    }
    return prev;
}

/*==========================================================
 *  Main entry function a(idx_t n)
 *==========================================================*/
seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;

    uint64_t m = (uint64_t)n;
    seq_t result = (seq_t)1;

    /* factor 2 */
    unsigned exp = 0;
    while ((m & 1ULL) == 0)
    {
        m >>= 1;
        ++exp;
    }
    if (exp) result *= prime_power_value(2, exp);

    /* factor odd primes */
    for (uint64_t p = 3; p <= m / p; p += 2)
    {
        if (m % p == 0)
        {
            exp = 0;
            while (m % p == 0)
            {
                m /= p;
                ++exp;
            }
            result *= prime_power_value(p, exp);
        }
    }

    /* remaining prime factor if any */
    if (m > 1) result *= prime_power_value(m, 1);

    return result;
}
