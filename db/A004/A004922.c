/*
 * A004922  a(n) = floor(n*phi^7), where phi is the golden ratio, A001622.
 * https://oeis.org/A004922
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*phi^7) = 29*n + floor(n/phi^7), since phi^7 = 29
 *            + phi^(-7); floor(n/phi^7) = floor((sqrt(845*n^2) - 29*n)/2) =
 *            floor((isqrt(845*n^2) - 29*n)/2)
 * Range:     u8 n <= 8, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned __int128 isqrt_u128(unsigned __int128 x) {
    unsigned __int128 r, y;
    if (x < 2) return x;
    r = x;
    y = (r + 1) / 2;
    while (y < r) {
        r = y;
        y = (r + x / r) / 2;
    }
    return r;
}

seq_t a(idx_t n) {
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 s = isqrt_u128((unsigned __int128)845 * nn * nn);
    uint64_t frac = (uint64_t)((s - 29 * nn) / 2);
    return (seq_t)n * 29 + (seq_t)frac;
}
