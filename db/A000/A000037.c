/*
 * A000037  Numbers that are not squares (or, the nonsquares).
 * https://oeis.org/A000037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, nice, changed
 * Formula:   a(n) = n + floor(1/2 + sqrt(n)), i.e. n + k where k is the
 *            largest integer with k*(k-1) < n
 * Range:     u8 n <= 240, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a000037_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a000037_isqrt.h"

seq_t a(idx_t n)
{
    uint64_t r = a000037_isqrt((uint64_t)n);
    seq_t k = (seq_t)r;
    /* round(sqrt(n)) = r+1 iff r*(r+1) < n */
    if ((seq_t)r * ((seq_t)r + 1) < (seq_t)n) k = k + 1;
    return (seq_t)n + k;
}
