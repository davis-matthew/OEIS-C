/*
 * A004971  a(n) = ceiling(n*phi^16), where phi is the golden ratio, A001622.
 * https://oeis.org/A004971
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(n*phi^16) = 2207*n - floor(n*psi^16), where
 *            phi^16 = (2207 + 987*sqrt(5))/2 and psi^16 = (2207 -
 *            987*sqrt(5))/2 = 2207 - phi^16; floor(n*psi^16) = floor((2207*n
 *            - floor(987*n*sqrt(5)) - 1)/2) for n >= 1, a(0) = 0
 * Range:     u8 n <= 0, u16 n <= 28, u32 n <= 28, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static unsigned __int128 isqrt_u128(unsigned __int128 x) {
    unsigned __int128 r = 0;
    int bit;
    for (bit = 63; bit >= 0; bit--) {
        unsigned __int128 t = r | ((unsigned __int128)1 << bit);
        if (t * t <= x) r = t;
    }
    return r;
}

seq_t a(idx_t n) {
    unsigned __int128 nn, m, k, q;
    if (n == 0) return (seq_t)0;
    nn = (unsigned __int128)n;
    /* m = floor(987*n*sqrt(5)) */
    m = isqrt_u128((unsigned __int128)4870845 * nn * nn);
    k = (unsigned __int128)2207 * nn - m;
    q = (k - 1) / 2;
    return (seq_t)n * 2207 - (seq_t)(uint64_t)q;
}
