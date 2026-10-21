/*
 * A213187  a(n) = (p+1)/2 if 4 | p+1, and p otherwise, where p is the least
 *          prime > n with 2(n+1)-p prime.
 * https://oeis.org/A213187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (p+1)/2 if p+1 ≡ 0 (mod 4), otherwise p, * where p is the
 *            least prime > n such that 2*(n+1) – p is prime
 * Range:     u8 n <= 236, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Search for the least prime p > n with 2*(n+1)-p prime. */
    for (idx_t p = n + 1; ; ++p)
    {
        if (!is_prime(p))
            continue;

        idx_t q = 2 * (n + 1) - p;
        if (q < 2u)            /* 2*(n+1)-p must be at least 2 to be prime. */
            continue;
        if (!is_prime(q))
            continue;

        /* Found the required prime p.  Compute the result. */
        return (seq_t)((p + 1u) % 4u == 0u ? (p + 1u) / 2u : p);
    }
}
