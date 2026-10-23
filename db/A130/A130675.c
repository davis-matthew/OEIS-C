/*
 * A130675  Factorial of bigomega(n).
 * https://oeis.org/A130675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = factorial(bigomega(n))
 * Range:     u8 n <= 63, u16 n <= 511, u32 n <= 8191, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   bigomega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/bigomega.h"

seq_t a(idx_t n)
{
    seq_t w = bigomega(n);
    seq_t r = 1;
    for (seq_t i = 2; i <= w; ++i)
        r *= i;
    return r;
}
