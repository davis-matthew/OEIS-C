/*
 * A135478  Given n, find the smallest prime p such that n(n+1)+p = q is a
 *          prime; then a(n) = q.
 * https://oeis.org/A135478
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*(n+1) + smallest prime p such that n*(n+1)+p is prime
 * Range:     u8 n <= 15, u16 n <= 254, u32 n <= 49, u64 n <= 9999,
 *            u128 n <= 9999, big any n
 * Helpers:   is_prime_big, is_prime_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_small.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* base = n*(n+1) */
    seq_t base = (seq_t)n * (((seq_t)n) + 1);

    /* search for the smallest prime p with base+p prime */
    for (uint64_t p = 2;; ++p)
    {
        if (!is_prime_small(p))
            continue;
        seq_t q = base + (seq_t)p;
        if (is_prime_big(q))
            return q;
    }
}
