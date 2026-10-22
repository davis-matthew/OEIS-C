/*
 * A000164  Number of partitions of n into 3 squares (allowing part zero).
 * https://oeis.org/A000164
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(i,j,k) : i >= j >= k >= 0, n = i^2 + j^2 + k^2};
 *            computed by direct search over k and j, checking whether the
 *            remainder is a perfect square
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a164_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a164_isqrt.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    uint64_t N = (uint64_t)n;
    uint64_t k, j;
    for (k = 0; 3 * k * k <= N; k++) {
        for (j = k; k * k + 2 * j * j <= N; j++) {
            uint64_t rem = N - k * k - j * j;
            uint64_t i = a164_isqrt(rem);
            if (i * i == rem && i >= j)
                count++;
        }
    }
    return count;
}
