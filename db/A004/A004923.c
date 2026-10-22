/*
 * A004923  a(n) = floor(n*phi^8), where phi is the golden ratio, A001622.
 * https://oeis.org/A004923
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*phi^8) = 46*n + floor((floor(sqrt(2205*n^2)) -
 *            45*n)/2), since phi^8 = (47 + 21*sqrt(5))/2
 * Range:     u8 n <= 5, u16 n <= 1395, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a004923_isqrt128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a004923_isqrt128.h"

seq_t a(idx_t n) {
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 s = a004923_isqrt128(nn * nn * 2205u);
    uint64_t t = (uint64_t)((s - nn * 45u) / 2u);
    return (seq_t)n * 46 + (seq_t)t;
}
