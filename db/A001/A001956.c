/*
 * A001956  Beatty sequence of (5+sqrt(13))/2.
 * https://oeis.org/A001956
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(5+sqrt(13))/2) = floor((5*n + isqrt(13*n^2))/2)
 * Range:     u8 n <= 59, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a1956_isqrt, a1956_isqrt128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a1956_isqrt.h"
#include "../utils/a1956_isqrt128.h"

seq_t a(idx_t n) {
    if (n < ((idx_t)1 << 60)) {
        unsigned __int128 N = (unsigned __int128)n;
        unsigned __int128 s = a1956_isqrt128((unsigned __int128)13 * N * N);
        unsigned __int128 r = (5 * N + s) / 2;
        return (seq_t)(uint64_t)r;
    } else {
        seq_t N = (seq_t)n;
        seq_t s = a1956_isqrt((seq_t)13 * N * N);
        return ((seq_t)5 * N + s) / 2;
    }
}
