/*
 * A104768  Number of matrices G with entries in Z satisfying G^2=G+1 and
 *          having the form 2G=[1+p q-2n | q+2n 1-p].
 * https://oeis.org/A104768
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = r₂(4·n² + 5), the number of representations of 4n²+5 as *
 *            a sum of two squares (order and sign counted)
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Compute m = 4n²+5 safely in 64‑bit arithmetic. */
    uint64_t nn = (uint64_t)n;
    uint64_t m  = 4ULL * nn * nn + 5ULL;

    /* r2 will accumulate the product of (exp+1) for primes ≡1 (mod 4). */
    uint64_t r2 = 1;
    uint64_t temp = m;

    /* Trial division up to sqrt(temp).  temp never exceeds 4·nn²+5 < 2³² for
     * the intended n range, so the loop fits comfortably within 32‑bit limits. */
    for (uint64_t p = 2; p * p <= temp; ++p)
    {
        if (temp % p == 0)
        {
            uint64_t exp = 0;
            while (temp % p == 0)
            {
                temp /= p;
                ++exp;
            }
            if ((p & 3ULL) == 3ULL)       /* p ≡ 3 (mod 4) */
            {
                if (exp & 1ULL)            /* odd exponent ⇒ no representations */
                    return (seq_t)0;
            }
            else if ((p & 3ULL) == 1ULL)  /* p ≡ 1 (mod 4) */
            {
                r2 *= (exp + 1ULL);
            }
            /* p == 2 is ignored, as it does not affect r₂. */
        }
    }

    /* Any remaining factor greater than 1 is prime. */
    if (temp > 1ULL)
    {
        if ((temp & 3ULL) == 3ULL)      /* 3 mod 4 prime remaining */
            return (seq_t)0;
        else if ((temp & 3ULL) == 1ULL) /* 1 mod 4 prime remaining */
            r2 *= 2ULL;                /* its exponent is 1 ⇒ (1+1) = 2 */
    }

    /* Final result: r₂(m) = 4·∏(exp_i+1) for primes ≡1 (mod 4). */
    r2 *= 4ULL;
    return (seq_t)r2;
}
