/*
 * A001951  A Beatty sequence: a(n) = floor(n*sqrt(2)).
 * https://oeis.org/A001951
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = floor(n*sqrt(2)) = A000196(2*n^2), the integer square
 *            root of 2*n^2
 * Range:     u8 n <= 181, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a001951_isqrt128, a001951_isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001951_isqrt128.h"
#include "../utils/a001951_isqrt_seq.h"

seq_t a(idx_t n)
{
    if (n < ((idx_t)1 << 63)) {
        unsigned __int128 v = (unsigned __int128)n * (unsigned __int128)n * 2;
        uint64_t r = (uint64_t)a001951_isqrt128(v);
        return (seq_t)r;
    } else {
        seq_t sn = (seq_t)n;
        return a001951_isqrt_seq(sn * sn * 2, sn * 2);
    }
}
