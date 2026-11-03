/*
 * A000093  a(n) = floor(n^(3/2)).
 * https://oeis.org/A000093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n^(3/2)) = A000196(n^3)
 * Range:     u8 n <= 40, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Validated: standard, pass, 74 terms through n = 472 (2026-11-01)
 * Helpers:   isqrt_seq, isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_seq.h"
#include "../utils/isqrt_u64.h"

seq_t a(idx_t n)
{
    if (n < ((idx_t)1 << 21)) {
        uint64_t c = (uint64_t)n * n * n;
        return (seq_t)isqrt_u64(c);
    }
    {
        seq_t N = (seq_t)n * (seq_t)n * (seq_t)n;
        return isqrt_seq(N);
    }
}
