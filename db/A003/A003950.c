/*
 * A003950  Expansion of g.f.: (1+x)/(1-7*x).
 * https://oeis.org/A003950
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(0) = 1; a(n) = 8*7^(n-1) for n >= 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 45,
 *            big any n
 * Helpers:   a000248_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a000248_pow.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    return (seq_t)8 * a000248_pow((seq_t)7, n - 1);
}
