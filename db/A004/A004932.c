/*
 * A004932  a(n) = floor(n*phi^17), where phi is the golden ratio, A001622.
 * https://oeis.org/A004932
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*phi^17) = floor((3571*n + sqrt(5*1597^2*n^2))/2),
 *            since phi^17 = (L(17) + F(17)*sqrt(5))/2
 * Range:     u8 n <= 0, u16 n <= 18, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a004932_isqrt128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a004932_isqrt128.h"

seq_t a(idx_t n) {
    unsigned __int128 k = (unsigned __int128)1597 * n;
    unsigned __int128 d = 5 * k * k;
    uint64_t m = a004932_isqrt128(d);
    unsigned __int128 r = ((unsigned __int128)3571 * n + m) / 2;
    return (seq_t)(uint64_t)r;
}
