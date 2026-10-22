/*
 * A000263  Number of partitions into non-integral powers.
 * https://oeis.org/A000263
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{x>=1} max(0, n^2 - ceil(2*n*sqrt(x))), i.e. for each
 *            x_1 = x, count x_2 with x < x_2 <= (n - sqrt(x))^2. Computed by
 *            direct summation over x with an exact integer square root
 * Range:     u8 n <= 7, u16 n <= 29, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   a263_ceil_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/a263_ceil_sqrt.h"

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n * (uint64_t)n;
    uint64_t four_nn = 4 * nn;
    seq_t sum = 0;
    uint64_t x;
    for (x = 1;; x++) {
        uint64_t c = a263_ceil_sqrt(four_nn * x);
        if (c >= nn) break;
        sum += (seq_t)(nn - c);
    }
    return sum;
}
