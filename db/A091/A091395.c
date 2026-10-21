/*
 * A091395  a(n) = Product_{ p | n } (1 + Legendre(-7,p) ).
 * https://oeis.org/A091395
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = Product_{ p | n } (1 + Legendre(-7,p) )
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static inline uint64_t mulmod64(uint64_t a, uint64_t b, uint64_t mod)
{
    return (unsigned __int128)a * b % mod;
}

/* Legendre symbol (−7|p) for odd prime p (returns -1, 0, or 1).
   For p = 2 the value is +1; for p = 7 the value is 0. */
static int legendre_minus7(uint64_t p)
{
    if (p == 2ULL) return 1;
    if (p == 7ULL) return 0;

    /* (-7 mod p) = (p - (7 mod p)) mod p */
    uint64_t base = (p - (7ULL % p)) % p;
    uint64_t exp  = (p - 1ULL) >> 1;          /* (p-1)/2 */
    uint64_t res  = 1;

    while (exp) {
        if (exp & 1ULL)
            res = mulmod64(res, base, p);
        base = mulmod64(base, base, p);
        exp >>= 1;
    }

    if (res == 1ULL)        return 1;
    if (res == p - 1ULL)    return -1;
    return 0;               /* only possible when p | 7, handled above */
}

seq_t a(idx_t n)
{
    if (n == 1ULL)
        return (seq_t)1;

    seq_t result = (seq_t)1;
    uint64_t tmp  = n;

    /* factor 2 */
    if ((tmp & 1ULL) == 0) {
        result *= (seq_t)2;
        while ((tmp & 1ULL) == 0)
            tmp >>= 1;
    }

    /* factor odd primes */
    for (uint64_t p = 3ULL; p * p <= tmp; p += 2ULL) {
        if (tmp % p == 0ULL) {
            int leg = legendre_minus7(p);
            result *= (seq_t)(1 + leg);
            while (tmp % p == 0ULL)
                tmp /= p;
        }
    }

    /* remaining prime factor > sqrt(tmp) */
    if (tmp > 1ULL) {
        int leg = legendre_minus7(tmp);
        result *= (seq_t)(1 + leg);
    }

    return result;
}
