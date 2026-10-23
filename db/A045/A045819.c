/*
 * A045819  Theta series of E_8 lattice with respect to midpoint of edge.
 * https://oeis.org/A045819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * σ₃(2n + 1) where σ₃(m) = Σ_{d|m} d³
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 33, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Note: all arithmetic on seq_t must use only the allowed operators.
   For any intermediate that may exceed seq_t limits we use unsigned
   __int128, which is available in both C11 and C++17. */
#include <stdint.h>

/*  Compute a^e with unsigned __int128 arithmetic.  e is small (≤48). */
static inline unsigned __int128 pow_uint64(uint64_t a, unsigned e)
{
    unsigned __int128 res = 1;
    unsigned __int128 base = a;
    while (e)
    {
        if (e & 1) res *= base;
        base *= base;
        e >>= 1;
    }
    return res;
}

/*  Compute σ₃(m) for m ≤ UINT64_MAX.
    Factor m by trial division, compute the product
    Π (p^{3(e+1)} – 1) / (p³ – 1). */
static inline unsigned __int128 sigma3_uint64(uint64_t m)
{
    if (m == 0) return 0;          /* σ₃(0) is undefined but never used */
    unsigned __int128 sigma = 1;
    uint64_t tmp = m;

    /* factor 2 separately if present – though m is odd (2n+1), keep general for safety */
    for (uint64_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p != 0) continue;
        unsigned e = 0;
        while (tmp % p == 0)
        {
            ++e; tmp /= p;
        }
        /* numerator = p^{3(e+1)} – 1; denominator = p³ – 1 */
        unsigned __int128 num = pow_uint64(p, 3 * (e + 1)) - 1;
        unsigned __int128 den = pow_uint64(p, 3) - 1;
        sigma = sigma * num / den;
    }
    /* remaining prime factor > sqrt(m) */
    if (tmp > 1)
    {
        unsigned e = 1;   /* exponent is 1 */
        unsigned __int128 num = pow_uint64(tmp, 3 * (e + 1)) - 1;
        unsigned __int128 den = pow_uint64(tmp, 3) - 1;
        sigma = sigma * num / den;
    }
    return sigma;
}

/* Public function required by the helper interface. */
seq_t a(idx_t n)
{
    /* compute m = 2n + 1  (uses unsigned __int128 to avoid overflow of 64‑bit multiply) */
    unsigned __int128 m128 = ((unsigned __int128)n) * 2 + 1;
    uint64_t m = (uint64_t)m128;   /* truncates if m exceeds UINT64_MAX – acceptable */
    unsigned __int128 s = sigma3_uint64(m);
    unsigned __int128 result = s * 2;   /* a(n) = 2 * sigma_3(m) */

    return (seq_t)result;
}
