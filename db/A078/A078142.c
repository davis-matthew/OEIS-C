/*
 * A078142  a(n) is the sum of the differences of the distinct prime factors p
 *          of n and the next square larger than p.
 * https://oeis.org/A078142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum over distinct prime factors p of n of ( (⌊√p⌋ + 1)² –
 *            p ) with a(1) = 0. The function factors n by trial division and
 *            accumulates the differences
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root of a 64‑bit unsigned integer,
   implemented with binary search and 128‑bit arithmetic.   */
static uint64_t floor_sqrt_u64(uint64_t v)
{
    uint64_t lo = 0, hi = ((uint64_t)1 << 32);   /* sqrt(2^64-1) < 2^32   */
    while (lo < hi) {
        uint64_t mid = (lo + hi + 1) >> 1;
        if ((unsigned __int128)mid * mid <= v)
            lo = mid;
        else
            hi = mid - 1;
    }
    return lo;
}

/* difference for a prime p: (next square > p) - p   */
static seq_t diff_for_prime(uint64_t p)
{
    uint64_t t = floor_sqrt_u64(p) + 1;   /* next integer > √p   */
    uint64_t d = (unsigned __int128)t * t - p;
    return (seq_t)d;
}

/* main sequence function */
seq_t a(idx_t n)
{
    if (n <= 1) return (seq_t)0;            /* a(1) = 0, a(0) undefined   */
    uint64_t temp = (uint64_t)n;
    seq_t ans = 0;

    for (uint64_t d = 2; d <= temp / d; ++d) {
        if (temp % d == 0) {
            /* d is a prime factor (distinct) */
            ans += diff_for_prime(d);
            while (temp % d == 0) temp /= d;
        }
    }
    if (temp > 1) {          /* remaining factor is prime > sqrt(original) */
        ans += diff_for_prime(temp);
    }

    return ans;
}
