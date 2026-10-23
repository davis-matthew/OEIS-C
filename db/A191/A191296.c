/*
 * A191296  Least k such that k-1 and k+1 in binary representation have same
 *          number n of 0's as 1's.
 * https://oeis.org/A191296
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) = 2 * (2^(n-1) + 2) * (2^(n-1) - 1) for n ≥ 3; a(2) = 11
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 32, u128 n <= 64,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* The offset for the sequence is 2, so the first term
       is a(2) = 11. For n = 2 we return this constant.
       For n ≥ 3 the closed‑form expression holds. */
    if (n == 2)
        return (seq_t)11;

    /* Compute 2^(n-1).  pow2 is defined in the utilities
       and works for arbitrary–precision types. */
    seq_t t = pow2(n - 1);

    /* Compute the product 2 * (t + 2) * (t - 1).  All
       operations are performed in seq_t; no intermediate
       overflow may occur for any supported build. */
    seq_t part1 = t + (seq_t)2;
    seq_t part2 = t - (seq_t)1;
    seq_t result = (seq_t)2 * part1 * part2;

    return result;
}
