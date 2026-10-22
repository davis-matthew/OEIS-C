/*
 * A000925  Number of ordered ways of writing n as a sum of 2 squares of
 *          nonnegative integers.
 * https://oeis.org/A000925
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = number of ordered pairs (x, y) of nonnegative integers
 *            with x^2 + y^2 = n; found by searching x = 0..floor(sqrt(n)) and
 *            testing whether n - x^2 is a perfect square
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a925_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a925_isqrt.h"

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t s = a925_isqrt(N);
    uint64_t count = 0;
    uint64_t x;
    for (x = 0; x <= s; x++) {
        uint64_t rem = N - x * x;
        uint64_t y = a925_isqrt(rem);
        if (y * y == rem) count++;
    }
    return (seq_t)count;
}
