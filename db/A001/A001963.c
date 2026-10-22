/*
 * A001963  Winning positions in the u-pile of the 4-Wythoff game with i=1.
 * https://oeis.org/A001963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor((n+1/4)*(sqrt(5)-1)) = floor((isqrt(5*m^2) - m)/4)
 *            with m = 4n+1
 * Range:     u8 n <= 206, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a001963_isqrt128, a001963_isqrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001963_isqrt128.h"
#include "../utils/a001963_isqrt_seq.h"

seq_t a(idx_t n)
{
    if (n < ((idx_t)1 << 58)) {
        unsigned __int128 m = (unsigned __int128)n * 4 + 1;
        unsigned __int128 s = a001963_isqrt128(m * m * 5);
        uint64_t r = (uint64_t)((s - m) / 4);
        return (seq_t)r;
    } else {
        seq_t m = (seq_t)n * 4 + 1;
        seq_t s = a001963_isqrt_seq(m * m * 5);
        return (s - m) / 4;
    }
}
