/*
 * A002055  Number of diagonal dissections of a convex n-gon into n-4 regions.
 * https://oeis.org/A002055
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = binomial(n-3, 2)*binomial(2*n-6, n-5)/(n-4) =
 *            (n-3)*binomial(2*n-6, n-5)/2
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 34, u128 n <= 66,
 *            big any n
 * Helpers:   a2055_binom, a2055_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a2055_binom.h"
#include "../utils/a2055_gcd.h"

seq_t a(idx_t n) {
    idx_t n3 = n - 3;
    seq_t b = a2055_binom(2 * n - 6, n - 5);
    if (n3 % 2 == 0)
        return (seq_t)(n3 / 2) * b;
    return (b / 2) * (seq_t)n3;
}
