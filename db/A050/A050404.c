/*
 * A050404  Partial sums of A051878.
 * https://oeis.org/A050404
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = binomial(n+5,5)*(4*n+3)/3
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 81, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* binomial coefficient C(n+5,5) */
    seq_t bin = binomial(n + 5, 5);

    /* multiplier (4*n + 3) */
    seq_t mul = (seq_t)n * 4 + 3;

    /* product divided by 3 */
    seq_t prod = bin * mul;
    return prod / 3;
}
