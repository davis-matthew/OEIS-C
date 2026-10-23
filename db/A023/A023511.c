/*
 * A023511  Least odd prime divisor of prime(n) + 1, or 1 if prime(n) + 1 is a
 *          power of 2.
 * https://oeis.org/A023511
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least odd prime divisor of prime(n)+1, or 1 if prime(n)+1
 *            is a power of 2
 * Range:     u8 n <= 32, u16 n <= 87, u32 n <= 87, u64 n <= 87, u128 n <= 87,
 *            big any n
 * Helpers:   floor_sqrt, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Get the n-th prime (1‑based) and add one */
    seq_t p = (seq_t)get_prime(n);
    seq_t x = p + 1;

    /* Strip all factors of 2 */
    while ((x & 1) == 0) {
        x >>= 1;                /* equivalent to x /= 2 */
    }

    /* If the result is 1, the original number was a power of two */
    if (x == 1) {
        return 1;
    }

    /* Find the least odd prime divisor by trial division */
    seq_t sq = floor_sqrt(x);   /* floor(sqrt(x)) */
    seq_t d = 3;
    for ( ; d <= sq; d += 2 ) {
        if ( x % d == 0 ) {
            return d;
        }
    }

    /* x itself is prime and odd, so it is the least odd prime divisor */
    return x;
}
