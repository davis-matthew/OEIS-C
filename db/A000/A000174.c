/*
 * A000174  Number of partitions of n into 5 squares.
 * https://oeis.org/A000174
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(a,b,c,d,e) : 0 <= a <= b <= c <= d <= e,
 *            a^2+b^2+c^2+d^2+e^2 = n} (counted by a search over a,b,c,d, with
 *            e found by integer square root)
 * Range:     u8 n <= 1098, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   sq5_isqrt, sq5_isqrt_fast
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/sq5_isqrt.h"
#include "../utils/sq5_isqrt_fast.h"

seq_t a(idx_t n)
{
    uint64_t N = (uint64_t)n;
    uint64_t count = 0;
    uint64_t p, q, r, s;
    (void)sq5_isqrt;
    for (p = 0; 5 * p * p <= N; p++) {
        uint64_t s1 = p * p;
        for (q = p; s1 + 4 * q * q <= N; q++) {
            uint64_t s2 = s1 + q * q;
            for (r = q; s2 + 3 * r * r <= N; r++) {
                uint64_t s3 = s2 + r * r;
                for (s = r; s3 + 2 * s * s <= N; s++) {
                    uint64_t rem = N - s3 - s * s;
                    uint64_t e = sq5_isqrt_fast(rem);
                    if (e * e == rem && e >= s) count++;
                }
            }
        }
    }
    return (seq_t)count;
}
