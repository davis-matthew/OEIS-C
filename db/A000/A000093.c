/*
 * A000093  a(n) = floor(n^(3/2)).
 * https://oeis.org/A000093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^(3/2)) = A000196(n^3)
 * Range:     u8 n <= 40, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   a93_isqrt64, a93_isqrt_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a93_isqrt64.h"
#include "../utils/a93_isqrt_big.h"

seq_t a(idx_t n)
{
    if (n < ((idx_t)1 << 21)) {
        uint64_t c = (uint64_t)n * n * n;
        return (seq_t)a93_isqrt64(c);
    }
    {
        seq_t N = (seq_t)n * (seq_t)n * (seq_t)n;
        return a93_isqrt_big(N);
    }
}
