/*
 * A004946  a(n) = round(n*phi^11), where phi is the golden ratio, A001622.
 * https://oeis.org/A004946
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(n*phi^11) = floor((199*n + 1 +
 *            floor(sqrt(39605*n^2)))/2), since phi^11 = (199 + 89*sqrt(5))/2
 * Range:     u8 n <= 1, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned __int128 isqrt_u128(unsigned __int128 x) {
    unsigned __int128 res = 0;
    unsigned __int128 bit = (unsigned __int128)1 << 126;
    while (bit > x) bit >>= 2;
    while (bit != 0) {
        if (x >= res + bit) {
            x -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return res;
}

seq_t a(idx_t n) {
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 m = isqrt_u128((unsigned __int128)39605 * nn * nn);
    /* sqrt(39605 n^2) = 89 n sqrt(5) is irrational for n > 0, so its floor suffices */
    unsigned __int128 small = (1 + m - (unsigned __int128)199 * nn) / 2;
    seq_t r = (seq_t)n * 199;
    r += (seq_t)(uint64_t)small;
    return r;
}
