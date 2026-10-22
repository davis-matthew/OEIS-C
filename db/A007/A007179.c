/*
 * A007179  Dual pairs of integrals arising from reflection coefficients.
 * https://oeis.org/A007179
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n/2 - 2^(n/2)*(1+(-1)^n)/4, i.e. a(0) = 0, a(n) =
 *            2^(n-1) for odd n, a(n) = 2^(n-1) - 2^(n/2-1) for even n >= 2
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   a007179_p2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a007179_p2.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (n & 1) return a007179_p2(n - 1);
    return a007179_p2(n - 1) - a007179_p2(n / 2 - 1);
}
