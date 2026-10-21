/*
 * A087349  a(n) = n + (smallest prime-factor of n+1).
 * https://oeis.org/A087349
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + smallest_prime_factor(n+1)
 * Range:     u8 n <= 129, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static seq_t _spf_uint64(uint64_t x)
{
    if (x <= 1ULL)
        return (seq_t)x;               /* for n = 0, n+1 = 1  (spf=1) */
    if ((x & 1ULL) == 0ULL)
        return (seq_t)2ULL;           /* even numbers */
    for (uint64_t d = 3ULL; d * d <= x; d += 2ULL)
        if (x % d == 0ULL)
            return (seq_t)d;          /* first divisor is the smallest prime factor */
    return (seq_t)x;                  /* x itself is prime */
}

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    uint64_t val = (uint64_t)(n) + 1ULL;   /* n+1 fits in 64 bits for the tested ranges */
    seq_t spf = _spf_uint64(val);
    return sn + spf;
}
