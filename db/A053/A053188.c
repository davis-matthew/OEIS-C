/*
 * A053188  Distance from n to nearest square.
 * https://oeis.org/A053188
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, changed
 * Formula:   a(n) = |floor(sqrt(n)+1/2)^2 - n|
 * Range:     u8 n <= 63, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    seq_t N = (seq_t)n;
    seq_t r = floor_sqrt(N);
    seq_t r2 = r * r;
    seq_t lowDiff = N - r2;
    seq_t r1 = r + (seq_t)1;
    seq_t r12 = r1 * r1;
    seq_t highDiff = r12 - N;
    return (lowDiff <= highDiff) ? lowDiff : highDiff;
}
