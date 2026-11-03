/*
 * A082902  a(n) = gcd(2^n, sigma(2,n)) = gcd(A000079(n), A001157(n)).
 * https://oeis.org/A082902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = gcd(2^n, σ₂(n)) = 2^min(n , v₂(σ₂(n))) where σ₂(n) =
 *            Σ_{d|n} d², and v₂(x) is the exponent of 2 in x
 * Range:     u8 n <= 65537, u16 n <= 65537, u32 n <= 65537, u64 n <= 65537,
 *            u128 n <= 65537, big any n
 * Validated: standard, pass, 121 terms through n = 9313 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>   /* for size_t */
#include <stdint.h>   /* for uint64_t */

/* Multiplicative computation of σ₂(n) and its 2‑adic valuation. */
static unsigned v2_of_sigma2(idx_t n)
{
    /* Factorize n by trial division (n ≤ ~10⁴ is cheap). */
    uint64_t m = (uint64_t)n;
    unsigned v2 = 0;                /* v₂(σ₂(n)) accumulator */

    /* σ₂ will be built in a 128‑bit integer to avoid overflow. */
    unsigned __int128 sigma2 = 1;
    uint64_t x = m;
    for (uint64_t p = 2; p * p <= x; ++p) {
        if (x % p == 0) {
            unsigned e = 0;
            while (x % p == 0) {
                x /= p;
                ++e;
            }
            /* term = (p^{2(e+1)} – 1) / (p² – 1) */
            unsigned __int128 powp = 1;
            for (unsigned i = 0; i < 2 * (e + 1); ++i)
                powp *= p;
            unsigned __int128 numerator   = powp - 1;
            unsigned __int128 denominator = (unsigned __int128)p * p - 1;
            unsigned __int128 term = numerator / denominator;
            sigma2 *= term;
        }
    }
    /* Remaining prime factor, if any. */
    if (x > 1) {
        uint64_t p = x;          /* exponent e = 1 */
        unsigned __int128 powp = 1;
        for (unsigned i = 0; i < 4; ++i)   /* 2*(1+1)=4 */
            powp *= p;
        unsigned __int128 numerator   = powp - 1;
        unsigned __int128 denominator = (unsigned __int128)p * p - 1;
        unsigned __int128 term = numerator / denominator;
        sigma2 *= term;
    }

    /* Count exponent of 2 in sigma2. */
    unsigned exp = 0;
    unsigned __int128 tmp = sigma2;
    while ((tmp & 1) == 0 && tmp != 0) {
        tmp >>= 1;
        ++exp;
    }
    return exp;
}

/* Compute 2^exp as a seq_t value.
   It is safe for seq_t types that are integer or arithmetic. */
static inline seq_t pow2_seq_t(unsigned exp)
{
    seq_t r = 1;
    for (unsigned i = 0; i < exp; ++i)
        r = r * (seq_t)2;
    return r;
}

/* OEIS sequence A082902 */
seq_t a(idx_t n)
{
    if (n == 0)   /* Not required for the sequence, but makes the function total */
        return (seq_t)1;

    unsigned exp_sigma2 = v2_of_sigma2(n);
    unsigned exp = (exp_sigma2 < (unsigned)n) ? exp_sigma2 : (unsigned)n;
    return pow2_seq_t(exp);
}
