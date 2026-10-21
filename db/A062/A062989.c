/*
 * A062989  a(n) = binomial(n+6, 6) - n - 1.
 * https://oeis.org/A062989
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+6,6) - n - 1
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 116, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t res = binomial(n + 6, 6);
    res -= (seq_t)n;
    res -= 1;
    return res;
}
