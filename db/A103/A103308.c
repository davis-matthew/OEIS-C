/*
 * A103308  Reduced denominators of the central moments of the distribution of
 *          random line segments picked on a unit line segment.
 * https://oeis.org/A103308
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) =
 *            denominator((2^(n+3)+(-1)^n*2*(3*n+5))/((n+1)*(n+2)*3^(n+2)))
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   vpow_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/vpow_uint64.h"

/* ---------- Helper functions ---------- */

/* Fast modular exponentiation for unsigned 64‑bit values */
static inline uint64_t mod_pow_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    if (mod == 1) return 0;
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1)
        {
            unsigned __int128 temp = (unsigned __int128)result * base % mod;
            result = (uint64_t)temp;
        }
        unsigned __int128 temp = (unsigned __int128)base * base % mod;
        base = (uint64_t)temp;
        exp >>= 1;
    }
    return result;
}

/* Compute B modulo a given modulus:
   B = 2^(n+2) + (-1)^n * (3*n+5)   (mod modulus)   (B is always positive for n≥2) */
static inline uint64_t B_mod(uint64_t n, uint64_t modulus)
{
    if (modulus == 0) return 0;
    uint64_t pow2 = mod_pow_uint64(2ULL, n + 2ULL, modulus);
    uint64_t odd = (3ULL * n + 5ULL) % modulus;
    uint64_t term = (n & 1ULL) ? (modulus - odd) % modulus : odd; /* sign = (-1)^n */
    return (pow2 + term) % modulus;
}

/* Greatest common divisor for 64‑bit unsigned integers */
static inline uint64_t gcd_uint64(uint64_t a, uint64_t b)
{
    while (b)
    {
        uint64_t t = b;
        b = a % b;
        a = t;
    }
    return a;
}

/* Power of seq_t by binary exponentiation (seq_t supports * and == 0,1). */
static inline seq_t pow_seq_t(seq_t base, uint64_t exp)
{
    seq_t result = 1;
    while (exp)
    {
        if (exp & 1ULL) result *= base;
        base *= base;
        exp >>= 1;
    }
    return result;
}

/* ---------- Main function ---------- */

/* Compute a(n) for n ≥ 2. For n < 2 no defined value; return 0. */
seq_t a(idx_t n)
{
    if (n < 2ULL) return 0;                      /* undefined below offset */

    /* N = (n+1)(n+2)  (fits easily in 64 bits for n ≤ 10 000)   */
    uint64_t N = (uint64_t)(n + 1ULL) * (uint64_t)(n + 2ULL);

    /* Compute gcd between 2·B and N.  Use B modulo N to avoid huge numbers */
    uint64_t BmodN = B_mod(n, N);
    uint64_t twoBmodN = (BmodN << 1ULL) % N;      /* 2·B mod N */
    uint64_t g1 = gcd_uint64(twoBmodN, N);

    uint64_t N_reduced = N / g1;                  /* factor of N remaining in denominator */

    /* Exponent of 3 in N */
    int v3_N = vpow_uint64(n + 1ULL, 3ULL) + vpow_uint64(n + 2ULL, 3ULL);

    /* Exponent of 3 in B */
    int e3num = 0;
    uint64_t mod3pow = 3ULL;
    while (1)
    {
        uint64_t Bmod = B_mod(n, mod3pow);
        if (Bmod % mod3pow == 0)
        {
            ++e3num;
            /* Will not overflow for the tiny exponents occurring in this sequence */
            mod3pow *= 3ULL;
        }
        else
            break;
    }

    /* Remaining exponent of 3 in numerator after cancelling with N */
    int e3_remaining = (e3num > v3_N) ? (e3num - v3_N) : 0;

    /* Remaining exponent of 3 in denominator after cancelling with numerator */
    uint64_t exp3 = (n + 2ULL) - (uint64_t)e3_remaining;

    /* Compute 3^(exp3) as seq_t */
    seq_t pow3 = pow_seq_t(3, exp3);

    /* Final reduced denominator */
    seq_t result = (seq_t)N_reduced * pow3;
    return result;
}
