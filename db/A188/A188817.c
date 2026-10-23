/*
 * A188817  Number of primes between n-sqrt(n) and n+sqrt(n), inclusive.
 * https://oeis.org/A188817
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = number of primes between n–⌊√n⌋ and n+⌊√n⌋, inclusive.
 *            The implementation counts primes by testing every integer in the
 *            interval with the supplied primality test
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _floor_isqrt_u64, is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/_floor_isqrt_u64.h"
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    /* Compute the integer square root of n. */
    idx_t sq = _floor_isqrt_u64(n);

    /* Determine the interval limits. */
    idx_t start = (n > sq) ? (n - sq) : 0;
    if (start < 2)
        start = 2;
    idx_t end = n + sq;

    /* Count primes in the closed interval [start, end]. */
    seq_t cnt = 0;
    for (idx_t x = start; x <= end; ++x)
    {
        if (is_prime(x))
            ++cnt;
    }
    return cnt;
}
