/*
 * A063475  a(n) = Sum_{d | H(n)} d^2, where H(n) is the half-totient function
 *          (A023022).
 * https://oeis.org/A063475
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = Σ_{d | H(n)} d² where H(n) = #{1 ≤ k ≤ n/2 : gcd(k,n)=1}
 *            – the half‑totient function. The function below implements this
 *            directly using inclusion–exclusion for H(n) and a simple divisor
 *            enumeration for the sum of squares
 * Range:     u8 n <= 30, u16 n <= 432, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 78 terms through n = 966 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Helper: compute the number of integers ≤ m that are coprime to n.
   This is H(n) with m = floor(n/2). */
static uint64_t half_totient(uint64_t n)
{
    uint64_t m = n >> 1;                 /* floor(n/2) */
    if (n <= 1) return 1;                /* not used for n ≥ 3 but safe */

    /* factor n to obtain its distinct prime factors */
    uint64_t primes[15];
    int cnt = 0;
    uint64_t tmp = n;
    for (uint64_t p = 2; p * p <= tmp; ++p)
    {
        if (tmp % p == 0)
        {
            primes[cnt++] = p;
            while (tmp % p == 0) tmp /= p;
        }
    }
    if (tmp > 1) primes[cnt++] = tmp;

    /* Inclusion–exclusion over the distinct primes */
    uint64_t coprime = m;
    for (uint64_t mask = 1; mask < ((uint64_t)1 << cnt); ++mask)
    {
        uint64_t prod = 1;
        int bits = 0;
        for (int i = 0; i < cnt; ++i)
        {
            if (mask & ((uint64_t)1 << i))
            {
                prod *= primes[i];
                ++bits;
            }
        }
        uint64_t term = m / prod;
        if (bits & 1) coprime -= term;   /* subtract when odd number of primes */
        else          coprime += term;   /* add when even number of primes   */
    }
    return coprime;
}

/* Public function required by the build. */
seq_t a(idx_t n)
{
    if (n < 3) return 0;                /* offset is 3; behaviour below is irrelevant */

    uint64_t H = half_totient((uint64_t)n);  /* H(n) fits easily in 64‑bit */

    /* Sum of squares of all divisors of H. */
    seq_t sum = 0;
    for (idx_t d = 1; (uint64_t)d * (uint64_t)d <= H; ++d)
    {
        if (H % d == 0)
        {
            uint64_t d_sq = (uint64_t)d * (uint64_t)d;
            uint64_t other = H / d;
            if (other == d)              /* perfect square case */
            {
                sum += (seq_t)d_sq;
            }
            else
            {
                sum += (seq_t)d_sq;
                sum += (seq_t)(other * other);
            }
        }
    }
    return sum;
}
