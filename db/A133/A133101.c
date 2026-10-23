/*
 * A133101  Expansion of f(x^2, x^3) in powers of x where f(, ) is Ramanujan's
 *          general theta function.
 * https://oeis.org/A133101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 if (1 + 40*n) is a perfect square m with m % 10 == 1 or
 *            9, otherwise a(n) = 0
 * Range:     u8 n <= 1, u16 n <= 104, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    seq_t ns = (seq_t)n;
    seq_t D = (seq_t)1 + (seq_t)40 * ns;
    seq_t m = floor_sqrt(D);
    if (m * m != D)
        return (seq_t)0;
    seq_t r = m % (seq_t)10;
    if (r == (seq_t)1 || r == (seq_t)9)
        return (seq_t)1;
    return (seq_t)0;
}
