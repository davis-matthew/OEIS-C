/*
 * A054576  Largest proper factor of the largest proper factor of n.
 * https://oeis.org/A054576
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 1023, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = largest proper factor of the largest proper factor of n
 * with the convention that the largest proper factor of 1 is 1.
 */
#include <stdint.h>

/* Integer square‑root: floor(sqrt(n)) for idx_t (uint64_t). */
static idx_t _isqrt_u64(idx_t n)
{
    idx_t r = 0;
    /* iterate over bits from high to low */
    for (int b = 62; b >= 0; --b)
    {
        idx_t t = r | ((idx_t)1 << b);
        if ((__uint128_t)t * t <= n)
            r = t;
    }
    return r;
}

/* Smallest prime factor of n (returns n if n is prime or n<=1) */
static idx_t _smallest_prime_factor(idx_t n)
{
    if (n <= 1)
        return n;            /* n==1 → 1, n==0 → 0 */
    if ((n & 1ULL) == 0ULL)
        return 2ULL;
    idx_t limit = _isqrt_u64(n);
    for (idx_t d = 3ULL; d <= limit; d += 2ULL)
    {
        if (n % d == 0ULL)
            return d;
    }
    return n;               /* prime */
}

/* Largest proper factor of n (returns 1 for n==1) */
static idx_t _largest_proper_factor(idx_t n)
{
    if (n <= 1)
        return 1;            /* by definition */
    idx_t spf = _smallest_prime_factor(n);
    return n / spf;         /* largest proper factor = n / smallest prime factor */
}

seq_t a(idx_t n)
{
    if (n <= 1)
        return (seq_t)1;     /* a(1)=1; for n==0 we return 1 by convention */
    idx_t first  = _largest_proper_factor(n);
    idx_t second = _largest_proper_factor(first);
    return (seq_t)second;
}
