/*
 * A140211  a(n) = Product_{d == 3 (mod 4) and d|n} d.
 * https://oeis.org/A140211
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Product_{d == 3 (mod 4) and d|n} d
 * Range:     u8 n <= 54, u16 n <= 134, u32 n <= 674, u64 n <= 3464,
 *            u128 n <= 9979, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* The product of an empty set of divisors is defined as 1. */
    seq_t prod = 1;   /* seq_t is an unsigned integer type or arbitrary-precision class. */
    if (n < 3) {
        return prod;
    }
    /* Iterate over all potential divisors d ≡ 3 (mod 4) up to n. */
    for (idx_t d = 3; d <= n; d += 4) {
        if (n % d == 0) {
            prod *= (seq_t)d;
        }
    }
    return prod;
}
