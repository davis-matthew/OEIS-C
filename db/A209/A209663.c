/*
 * A209663  Numbers n with property that n is prime, n+18 is prime and both
 *          have equal sum of digits.
 * https://oeis.org/A209663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   Compute the nth term of A209663 by searching primes p such that
 *            p+18 is also prime and the sum of digits of p equals the sum of
 *            digits of p+18. The sequence is indexed starting at 1
 * Range:     u8 n <= 17, u16 n <= 52, u32 n <= 52, u64 n <= 52, u128 n <= 52,
 *            big any n
 * Helpers:   append_candidate, digit_sum, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>
#include "../utils/is_prime.h"
#include "../utils/digit_sum.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    /* Persistent cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;

    /* Next candidate prime to test (odd numbers only) */
    static idx_t next_candidate = 5;

    /* Ensure cache has at least n terms */
    while (cache_len < (size_t)n)
    {
        if (is_prime(next_candidate))
        {
            idx_t partner = next_candidate + (idx_t)18;  /* p + 18 */
            if (is_prime(partner))
            {
                seq_t sum1 = digit_sum((seq_t)next_candidate);
                seq_t sum2 = digit_sum((seq_t)partner);
                if (sum1 == sum2)
                {
                    /* Store the qualifying prime */
                    append_candidate(&cache, &cache_len, &cache_cap,
                                     (seq_t)next_candidate);
                }
            }
        }
        /* Advance to the next odd number */
        next_candidate += 2;
    }

    return cache[n - 1];
}
