/*
 * A096839  Number of primes in the neighborhood of n^2 with radius
 *          ceiling(log(n^2)).
 * https://oeis.org/A096839
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of primes in [n^2 - ceil(log(n^2)), n^2 +
 *            ceil(log(n^2))]
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)            /* the sequence is defined for n ≥ 1; return 0 for safety */
        return 0;

    /* square of n (fits in idx_t for n up to at least 10^8) */
    idx_t sq = (idx_t)n * (idx_t)n;

    /* radius = ceil(log(n^2)) = ceil(2 * log(n)) */
    idx_t r = (idx_t)ceil(2.0 * log((double)n));

    /* interval limits */
    idx_t L = (sq > r) ? sq - r : 0;
    idx_t R = sq + r;

    seq_t count = 0;
    for (idx_t x = L; x <= R; ++x)
        if (is_prime(x))
            ++count;

    return count;
}
