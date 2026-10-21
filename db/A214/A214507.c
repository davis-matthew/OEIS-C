/*
 * A214507  a(n) = 1 if n is one or two times an even square, -1 if one or two
 *          times an odd square else 0.
 * https://oeis.org/A214507
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = 1 if n is one or two times an even square, -1 if one or
 *            two times an odd square, else 0
 * Range:     i8 n <= 63, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;

    /* Check n = k^2 */
    seq_t k = floor_sqrt(N);
    if (k * k == N) {
        return (k & 1) ? (seq_t)-1 : (seq_t)1;
    }

    /* Check n = 2 * k^2 (n must be even) */
    if ((n & 1ULL) == 0) {
        seq_t half = N / 2;
        seq_t k2 = floor_sqrt(half);
        if (k2 * k2 == half) {
            return (k2 & 1) ? (seq_t)-1 : (seq_t)1;
        }
    }

    return (seq_t)0;
}
