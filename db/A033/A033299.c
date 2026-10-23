/*
 * A033299  Smallest safe prime ((p-1)/2 is also prime) > n.
 * https://oeis.org/A033299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 226, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the smallest safe prime greater than n.
   A safe prime p satisfies that p is prime and (p-1)/2 is also prime. */
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* Start searching from the integer just larger than n. */
    seq_t p = (seq_t)n + 1;
    for (;; ++p)
    {
        if (!is_prime_big(p))
            continue;
        /* (p-1)/2 is an integer for odd p ≥ 5. */
        seq_t q = (p - 1) / 2;
        if (is_prime_big(q))
            return p;
    }
}
