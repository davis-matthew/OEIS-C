/*
 * A071523  Number of 11-smooth numbers <= n.
 * https://oeis.org/A071523
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = #{k ≤ n | k is an 11‑smooth number} (i.e. k = 2^x · 3^y ·
 *            5^z · 7^u · 11^v for non‑negative integers x,y,z,u,v)
 * Range:     u8 n <= 1847, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>     /* for size_t */

static const unsigned long long _primes[5] = { 2ULL, 3ULL, 5ULL, 7ULL, 11ULL };

/* Recursively count all 11‑smooth numbers ≤ limit that use only primes
   _primes[idx…4].  The current product is cur (initially 1).  The
   function returns the number of valid numbers for this branch. */
static unsigned long long _smooth_count_rec(unsigned long long limit,
                                            size_t idx,
                                            unsigned long long cur)
{
    if (idx == 5)                /* all primes processed → one valid number */
        return 1ULL;

    unsigned long long count = 0ULL;
    const unsigned long long p = _primes[idx];

    /* Enumerate exponent values for prime p. */
    while (cur <= limit) {
        count += _smooth_count_rec(limit, idx + 1, cur);

        /* Stop if the next multiplication would exceed the limit (or
           overflow in the unsigned long long type). */
        if (cur > limit / p)
            break;
        cur *= p;
    }
    return count;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* defined behaviour for indices below 1 */

    unsigned long long limit = (unsigned long long)n;
    unsigned long long cnt   = _smooth_count_rec(limit, 0, 1ULL);
    return (seq_t)cnt;
}
