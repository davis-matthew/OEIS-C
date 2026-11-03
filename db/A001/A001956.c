/*
 * A001956  Beatty sequence of (5+sqrt(13))/2.
 * https://oeis.org/A001956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(5+sqrt(13))/2) = floor((5*n + isqrt(13*n^2))/2)
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 74 terms through n = 8413 (2026-11-01)
 * Helpers:   isqrt_seq, isqrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_seq.h"
#include "../utils/isqrt_u128.h"

seq_t a(idx_t n) {
    if (n < ((idx_t)1 << 60)) {
        unsigned __int128 N = (unsigned __int128)n;
        unsigned __int128 s = isqrt_u128((unsigned __int128)13 * N * N);
        unsigned __int128 r = (5 * N + s) / 2;
        return (seq_t)(uint64_t)r;
    } else {
        seq_t N = (seq_t)n;
        seq_t s = isqrt_seq((seq_t)13 * N * N);
        return ((seq_t)5 * N + s) / 2;
    }
}
