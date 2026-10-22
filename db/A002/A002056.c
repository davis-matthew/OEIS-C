/*
 * A002056  Number of diagonal dissections of a convex n-gon into n-5 regions.
 * https://oeis.org/A002056
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n-3, 3)*binomial(2n-7, n-6)/(n-5) =
 *            ((n-3)*(n-4)/2) * binomial(2n-7, n-6) / 3
 * Range:     u8 n <= 8, u16 n <= 11, u32 n <= 18, u64 n <= 33, u128 n <= 64,
 *            big any n
 * Helpers:   a2056_binom, a2056_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a2056_binom.h"
#include "../utils/a2056_gcd.h"

seq_t a(idx_t n) {
    seq_t B = a2056_binom(2 * n - 7, n - 6);
    seq_t m = (seq_t)(n - 3) * (seq_t)(n - 4) / 2;
    if (m % 3 == 0) {
        m /= 3;
    } else {
        B /= 3;
    }
    return m * B;
}
