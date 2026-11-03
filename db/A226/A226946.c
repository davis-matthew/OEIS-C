/*
 * A226946  Numbers that can't be written as x^2 + x*y + y^2, with 0 <= x <= y
 *          and gcd(x,y) = 1.
 * https://oeis.org/A226946
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = the n-th integer ≥ 2 that is either divisible by 9 or has
 *            a prime divisor congruent to 2 modulo 3. This is equivalent to
 *            “numbers that cannot be written as x^2 + x*y + y^2 with 0 ≤ x ≤
 *            y and gcd(x, y) = 1”
 * Range:     u8 n <= 211, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9158 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static bool has_bad_factor(idx_t v)
{
    /* Divisible by 9 → not representable. */
    if (v % 9u == 0u)
        return true;

    idx_t t = v;
    /* Trial division to find a prime factor p ≡ 2 (mod 3). */
    for (idx_t p = 2; p * p <= t; ++p)
    {
        if (t % p == 0u)
        {
            if (p % 3u == 2u)
                return true;          /* bad prime factor found */

            /* Remove all occurrences of p. */
            while (t % p == 0u)
                t /= p;
        }
    }

    /* Remaining factor > 1, check it. */
    if (t > 1u && t % 3u == 2u)
        return true;

    return false;                     /* representable */
}

seq_t a(idx_t n)
{
    idx_t count = 0u;
    idx_t m = 2u;                     /* start at 2 (offset 1) */

    while (count < n)
    {
        if (has_bad_factor(m))
        {
            ++count;
            if (count == n)
                return (seq_t)m;      /* found the n-th term */
        }
        ++m;
    }
    return (seq_t)0;                  /* never reached */
}
