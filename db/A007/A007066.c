/*
 * A007066  a(n) = 1 + ceiling((n-1)*phi^2), phi = (1+sqrt(5))/2.
 * https://oeis.org/A007066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(1) = 1; a(n) = floor((n-1)*phi^2) + 2 for n >= 2, where
 *            floor(m*phi^2) = floor(m*phi) + m and floor(m*phi) = floor((m +
 *            isqrt(5*m^2))/2)
 * Range:     u8 n <= 98, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a007066_isqrt128, a007066_isqrt_seq, seq_is_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_is_big.h"
#include "../utils/u128_to_seq.h"
#include "../utils/a007066_isqrt128.h"
#include "../utils/a007066_isqrt_seq.h"

seq_t a(idx_t n) {
    uint64_t m;
    if (n <= 1) return (seq_t)1;
    m = (uint64_t)(n - 1);
    if (seq_is_big()) {
        seq_t M = (seq_t)m;
        seq_t N = M * M * 5;
        seq_t r = a007066_isqrt_seq(N);
        seq_t f = (M + r) / 2;
        return f + M + 2;
    } else {
        unsigned __int128 M = (unsigned __int128)m;
        unsigned __int128 N = M * M * 5;
        unsigned __int128 r = a007066_isqrt128(N);
        unsigned __int128 f = (M + r) / 2;
        return u128_to_seq(f + M + 2);
    }
}
