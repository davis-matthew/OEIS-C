/*
 * A224712  The number of unordered partitions {a, b} of n such that a or b is
 *          composite and the other is prime.
 * https://oeis.org/A224712
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{p ∈ primes | n-p is composite and n-p ≥ 4}
 * Range:     u8 n <= 1624, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n <= 3) return 0;          /* no partition can satisfy the conditions */

    seq_t count = 0;

    /* iterate over prime numbers p < n */
    for (idx_t p = 2; p <= n - 2; ++p)
    {
        if (!is_prime(p))
            continue;

        idx_t q = n - p;
        if (q < 4)          /* 1, 2, or 3 are not composite */
            continue;
        if (is_prime(q))    /* both would be prime, not allowed */
            continue;

        /* p is prime, q is composite → valid unordered pair (q,p) */
        count++;
    }
    return count;
}
