/*
 * A136415  Numbers k such that a type-3 Gaussian normal basis over GF(2^k)
 *          exists.
 * https://oeis.org/A136415
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 3563, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: search for k such that
   p = 3k+1 is prime and the multiplicative order of 2 modulo p is
   either p−1 (2 is a primitive root) or k (when k is not divisible by 3).
   The nth term a(n) is the nth such k starting with a(1)=4. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

static inline uint64_t pow_mod_uint64(uint64_t base, uint64_t exp, uint64_t mod)
{
    uint64_t result = 1 % mod;
    uint64_t b = base % mod;
    while (exp)
    {
        if (exp & 1ULL)
            result = (uint64_t)((unsigned __int128)result * b % mod);
        b = (uint64_t)((unsigned __int128)b * b % mod);
        exp >>= 1ULL;
    }
    return result;
}

static inline uint64_t multiplicative_order(uint64_t p)
{
    uint64_t ord = p - 1;
    uint64_t m   = p - 1;

    for (uint64_t f = 2ULL; f * f <= m; ++f)
    {
        if (m % f == 0ULL)
        {
            while (m % f == 0ULL)
                m /= f;
            while (ord % f == 0ULL &&
                   pow_mod_uint64(2ULL, ord / f, p) == 1ULL)
                ord /= f;
        }
    }
    if (m > 1ULL)
    {
        uint64_t f = m;
        while (ord % f == 0ULL &&
               pow_mod_uint64(2ULL, ord / f, p) == 1ULL)
            ord /= f;
    }
    return ord;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* undefined for offset 1; safeguard */

    uint64_t target = (uint64_t)n;
    uint64_t found  = 0;

    for (uint64_t k = 1ULL; ; ++k)
    {
        uint64_t p = 3ULL * k + 1ULL;
        if (!is_prime(p))
            continue;

        uint64_t ord = multiplicative_order(p);

        if (ord == (p - 1ULL) ||
            (ord == k && (k % 3ULL) != 0ULL))
        {
            ++found;
            if (found == target)
                return (seq_t)k;
        }
    }
}
