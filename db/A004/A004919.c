/*
 * A004919  a(n) = floor(n*phi^4), where phi is the golden ratio, A001622.
 * https://oeis.org/A004919
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*phi^4) = floor((7*n + sqrt(45*n^2))/2), since
 *            phi^4 = (7 + 3*sqrt(5))/2
 * Range:     u8 n <= 37, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/u128_to_seq.h"

static unsigned __int128 isqrt_u128(unsigned __int128 x) {
    unsigned __int128 r = 0;
    int b;
    for (b = 63; b >= 0; b--) {
        unsigned __int128 t = r | ((unsigned __int128)1 << b);
        if (t * t <= x)
            r = t;
    }
    return r;
}

seq_t a(idx_t n) {
    unsigned __int128 nn = (unsigned __int128)n;
    unsigned __int128 s = isqrt_u128(45 * nn * nn);
    /* sqrt(45 n^2) is irrational for n > 0, so its floor gives the exact floor */
    unsigned __int128 v = (7 * nn + s) / 2;
    return u128_to_seq(v);
}
