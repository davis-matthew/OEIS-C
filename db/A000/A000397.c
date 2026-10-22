/*
 * A000397  Number of partitions into non-integral powers.
 * https://oeis.org/A000397
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(x1,x2,x3): 1<=x1<x2<x3, sqrt(x1)+sqrt(x2)+sqrt(x3) <=
 *            n}. Counted by brute force: loop over x1, then a two-pointer
 *            sweep over x2 (going up) and the largest allowed x3 (going
 *            down). Each test is exact, using integer squaring in 128-bit
 *            arithmetic
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   le, le_exact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include "../utils/le.h"
#include "../utils/le_exact.h"

seq_t a(idx_t n)
{
    seq_t total = 0;
    uint64_t N = (uint64_t)n;
    uint64_t x;
    for (x = 1; le(x, x + 1, x + 2, N); x++) {
        uint64_t y = x + 1;
        double t = (double)N - sqrt((double)x) - sqrt((double)y);
        uint64_t c = (uint64_t)(t * t) + 2;
        uint64_t cnt = 0;
        for (;;) {
            while (c > y && !le(x, y, c, N)) c--;
            if (c <= y) break;
            cnt += c - y;
            y++;
        }
        total += (seq_t)cnt;
    }
    return total;
}
