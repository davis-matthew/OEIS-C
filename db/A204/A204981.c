/*
 * A204981  Least 2^(k-1) such that n divides 2^(k-1)-2^(j-1) for some j<k.
 * https://oeis.org/A204981
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 18, u32 n <= 36, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   mul_mod64, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS A204981
   a(n) = 2^(v2(n) + ord_{n/2^v2(n)}(2))
   where v2(n) is the exponent of 2 in n and
   ord_m(2) is the multiplicative order of 2 modulo m (for odd m).   */

#include <stdint.h>
#include "../utils/mul_mod64.h"   /* mul_mod64(uint64_t, uint64_t, uint64_t) */
#include "../utils/pow2.h"        /* pow2(idx_t) -> seq_t */

static inline uint64_t modpow64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    base %= mod;
    while (exp)
    {
        if (exp & 1ULL)
            result = mul_mod64(result, base, mod);
        base = mul_mod64(base, base, mod);
        exp >>= 1ULL;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Separate powers of 2 */
    uint64_t nn = (uint64_t)n;          /* n fits in 64 bits */
    unsigned int v2 = 0;
    while ((nn & 1ULL) == 0ULL)
    {
        nn >>= 1;
        ++v2;
    }
    uint64_t odd = nn;

    /* If n is a power of 2, the answer is 2^(v2+1) */
    if (odd == 1ULL)
        return pow2((idx_t)(v2 + 1));

    /* Compute Euler's totient of odd part to bound the order */
    uint64_t phi = odd;
    uint64_t t = odd;
    for (uint64_t p = 3ULL; p * p <= t; p += 2ULL)
    {
        if (t % p == 0ULL)
        {
            phi -= phi / p;
            while (t % p == 0ULL)
                t /= p;
        }
    }
    if (t > 1ULL)
        phi -= phi / t;

    /* Find the smallest divisor of phi such that 2^d ≡ 1 (mod odd) */
    uint64_t order = 0;
    for (uint64_t d = 1ULL; d <= phi; ++d)
    {
        if (phi % d != 0ULL)
            continue;
        if (modpow64(2ULL, d, odd) == 1ULL)
        {
            order = d;
            break;
        }
    }

    /* Final answer: 2^(v2 + order) */
    return pow2((idx_t)(v2 + order));
}
