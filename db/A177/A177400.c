/*
 * A177400  a(n) = binomial(n*2^n, n).
 * https://oeis.org/A177400
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binomial(n*2^n, n)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 7, u128 n <= 10,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Compute N = n * 2^n as seq_t */
    seq_t N = (seq_t)n * pow2(n);

    /* Compute binomial coefficient C(N, n) multiplicatively */
    seq_t res = 1;   /* 1 is a literal convertible to seq_t */

    for (idx_t i = 1; i <= n; ++i) {
        seq_t numerator = N - (seq_t)n + (seq_t)i; /* N - n + i */
        res = (res * numerator) / (seq_t)i;
    }

    return res;
}
