/*
 * A001964  v-pile positions of the 4-Wythoff game with i=1.
 * https://oeis.org/A001964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( (n+1/4)*(3+sqrt(5)) ) = floor( (3*m +
 *            floor(sqrt(5*m^2)))/4 ), m = 4n+1
 * Range:     u8 n <= 48, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 72 terms through n = 9933 (2026-11-01)
 * Helpers:   isqrt_seq, isqrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/isqrt_seq.h"
#include "../utils/isqrt_u128.h"

seq_t a(idx_t n) {
    if (n < ((idx_t)1 << 60)) {
        unsigned __int128 m = (unsigned __int128)4 * n + 1;
        unsigned __int128 t = isqrt_u128(5 * m * m);
        unsigned __int128 r = (3 * m + t) / 4;
        return (seq_t)(uint64_t)r;
    }
    seq_t m = (seq_t)n * 4 + 1;
    seq_t t = isqrt_seq(m * m * 5);
    return (m * 3 + t) / 4;
}
