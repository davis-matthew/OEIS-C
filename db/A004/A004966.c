/*
 * A004966  a(n) = ceiling(n*phi^11), where phi is the golden ratio, A001622.
 * https://oeis.org/A004966
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(n*phi^11) = floor((199*n +
 *            floor(sqrt(39605*n^2)))/2) + 1 for n >= 1, a(0) = 0, since
 *            phi^11 = (199 + 89*sqrt(5))/2 and 89^2*5 = 39605
 * Range:     u8 n <= 1, u16 n <= 34, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/u128_to_seq.h"

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
    if (n == 0) return u128_to_seq((unsigned __int128)0);
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 s = isqrt_u128((unsigned __int128)39605 * nn * nn);
    unsigned __int128 t = (unsigned __int128)199 * nn + s;
    return u128_to_seq(t / 2 + 1);
}
