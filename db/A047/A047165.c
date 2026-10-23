/*
 * A047165  Number of nonempty subsets of {1,2,...,n} in which exactly 1/5 of
 *          the elements are <= n/2.
 * https://oeis.org/A047165
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>=1} binomial(floor(n/2), k) * binomial(ceil(n/2),
 *            4*k)
 * Range:     u8 n <= 14, u16 n <= 24, u32 n <= 45, u64 n <= 86, u128 n <= 168,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"

/* Compute the sequence a(n) for n >= 1. */
seq_t a(idx_t n)
{
    /* For n < 7 the result is 0, the loop will handle it naturally. */
    idx_t half_floor = n / 2;          /* floor(n/2)   */
    idx_t half_ceil  = (n + 1) / 2;    /* ceil(n/2)    */

    /* k must satisfy k <= floor(half_floor) and 4*k <= half_ceil.
       Hence k <= min(half_floor, half_ceil/4). */
    idx_t k_limit = half_floor;
    idx_t tmp = half_ceil / 4;
    if (tmp < k_limit)
        k_limit = tmp;

    seq_t result = 0;
    for (idx_t k = 1; k <= k_limit; ++k) {
        seq_t part1 = binomial(half_floor, k);
        seq_t part2 = binomial(half_ceil, 4 * k);
        result += part1 * part2;
    }
    return result;
}
