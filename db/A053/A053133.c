/*
 * A053133  One half of binomial coefficients binomial(2*n-8,9).
 * https://oeis.org/A053133
 *
 * Offset:    9    (a(9) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(2*n-8, 9)/2, with offset 9 (a(9)=5)
 * Range:     u8 n <= 10, u16 n <= 13, u32 n <= 30, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   binom_9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom_9.h"

seq_t a(idx_t n)
{
    if (n < 9) return 0;
    idx_t N = 2 * n - 8;
    seq_t val = binom_9(N);
    return val / 2;
}
