/*
 * A218008  Sum of successive absolute differences of the binomial
 *          coefficients = 2*A014495(n).
 * https://oeis.org/A218008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(C(n, floor((n+1)/2)) - 1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute k = floor((n+1)/2) */
    idx_t k = (n + 1) / 2;

    /* Binomial coefficient C(n, k) */
    seq_t c = binomial(n, k);

    /* a(n) = 2 * (c - 1) */
    seq_t result = c - (seq_t)1;
    result <<= 1;      /* multiply by 2 */

    return result;
}
