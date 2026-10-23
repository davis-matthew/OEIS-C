/*
 * A142564  Primes congruent to 34 mod 53.
 * https://oeis.org/A142564
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   primes congruent to 34 mod 53
 * Range:     u8 n <= 1, u16 n <= 118, u32 n <= 121, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime p with p % 53 == 34, with offset 1. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

static inline seq_t candidate(idx_t k)
{
    return 34 + (seq_t)53 * (seq_t)k;
}

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1.  For n==0 we return 0. */
    if (n == 0)
        return 0;

    idx_t count = 0;
    idx_t k = 0;

    for (;; ++k)
    {
        seq_t val = candidate(k);
        if (is_prime_big(val))
        {
            ++count;
            if (count == n)
                return val;
        }
    }
}
