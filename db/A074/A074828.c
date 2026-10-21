/*
 * A074828  a(1) = 1; for n>1, a(n) = smallest composite multiple of n if n is
 *          a prime else the smallest prime divisor of n if n is composite.
 * https://oeis.org/A074828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for n > 1: if n is prime then a(n) = 2*n, else a(n) =
 *            smallest prime divisor of n
 * Range:     u8 n <= 130, u16 n <= 9912, u32 n <= 9912, u64 n <= 9912,
 *            u128 n <= 9912, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)1;

    /* Prime case */
    if (is_prime(n)) {
        return (seq_t)((seq_t)2 * (seq_t)n);
    }

    /* Composite case: find the smallest prime divisor */
    if ((n & 1) == 0)         /* even */
        return (seq_t)2;

    uint64_t limit = sqrtl((long double)n);
    for (uint64_t d = 3; d <= limit; d += 2) {
        if (n % d == 0)
            return (seq_t)d;
    }

    /* If no divisor was found, n is prime (should not happen) */
    return (seq_t)n;
}
