/*
 * A020484  Least prime p such that there exists a prime q with p-2n = q.
 * https://oeis.org/A020484
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the least prime p such that p-2n is also prime
 * Range:     u8 n <= 120, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* start at the smallest integer > 2n that could be a prime */
    idx_t start = 2 * n + 2;
    if (start < 2) start = 2;

    for (idx_t p = start; ; ++p) {
        if (is_prime(p) && is_prime(p - 2 * n))
            return (seq_t)p;
    }
}
