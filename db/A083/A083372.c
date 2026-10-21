/*
 * A083372  Least number having exactly two odd prime factors that differ by
 *          2n.
 * https://oeis.org/A083372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p * (p + 2n) where p is the smallest odd prime such that
 *            * p + 2n is also prime. The function searches for this pair by
 *            testing * successive odd primes
 * Range:     u8 n <= 14, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* 2n, cast to seq_t to avoid overflow */
    seq_t diff = (seq_t)2 * (seq_t)n;

    /* Start from the first odd prime, 3, and test subsequent odd numbers. */
    for (seq_t p = (seq_t)3; ; p += (seq_t)2)
    {
        if (is_prime_big(p) && is_prime_big(p + diff))
            return p * (p + diff);
    }
}
