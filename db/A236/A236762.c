/*
 * A236762  Number of partitions of 3n into 3 parts with the middle part
 *          prime.
 * https://oeis.org/A236762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{i=1..n} i * A010051(i) + Sum_{i=1..floor((n - 1)/2)}
 *            A010051(n + i) * (n - 2i)
 * Range:     u8 n <= 35, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    seq_t total = 0;

    /* Sum over i = 1..n of i if i is prime */
    for (idx_t i = 1; i <= n; ++i)
    {
        if (is_prime(i))
            total += (seq_t)i;
    }

    /* Sum over i = 1..floor((n-1)/2) of (n-2i) if n+i is prime */
    idx_t limit = (n - 1) / 2;
    for (idx_t i = 1; i <= limit; ++i)
    {
        if (is_prime(n + i))
            total += (seq_t)(n - 2 * i);
    }

    return total;
}
