/*
 * A062759  Largest power of squarefree kernel of n (= A007947) which divides
 *          n.
 * https://oeis.org/A062759
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (product of distinct prime divisors of n) ^ (min exponent
 *            of those primes)
 * Range:     u8 n <= 255, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

/* Integer square‑root for 64‑bit unsigned integers.
   (Result is floor(sqrt(n))) */
static inline uint64_t isqrt_u64(uint64_t n)
{
    uint64_t r = 0;
    while ((r + 1) * (r + 1) <= n)
        ++r;
    return r;
}

seq_t a(idx_t n)
{
    if (n <= 1)                 /* a(1) = 1 by definition */
        return (seq_t)1;

    uint64_t m = (uint64_t)n;
    seq_t    k = 1;            /* square‑free kernel */
    uint64_t min_exp = UINT64_MAX;
    uint64_t temp = m;
    const uint64_t m_sqrt = isqrt_u64(m);

    /* handle factor 2 */
    if (temp % 2 == 0) {
        uint64_t exp = 0;
        while (temp % 2 == 0) {
            temp /= 2;
            exp++;
        }
        k *= (seq_t)2;
        if (exp < min_exp) min_exp = exp;
    }

    /* odd prime factors up to sqrt(m) */
    for (uint64_t p = 3; p <= m_sqrt; p += 2) {
        if (temp % p == 0) {
            uint64_t exp = 0;
            while (temp % p == 0) {
                temp /= p;
                exp++;
            }
            k *= (seq_t)p;
            if (exp < min_exp) min_exp = exp;
        }
    }

    /* remaining prime factor > sqrt(m) */
    if (temp > 1) {
        k *= (seq_t)temp;
        if (1 < min_exp) min_exp = 1;
    }

    /* compute k ^ min_exp  (k ≤ n ≤ 10000, so result is ≤ n) */
    seq_t result = 1;
    for (uint64_t i = 0; i < min_exp; ++i)
        result *= k;

    return result;
}
