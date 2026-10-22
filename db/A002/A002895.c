/*
 * A002895  Domb numbers: number of 2n-step polygons on diamond lattice.
 * https://oeis.org/A002895
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice, walk, changed
 * Formula:   a(n) = Sum_{k=0..n}
 *            binomial(n,k)^2*binomial(2n-2k,n-k)*binomial(2k,k) (fixed-width
 *            builds); for arbitrary precision: n^3*a(n) =
 *            2*(2*n-1)*(5*n^2-5*n+2)*a(n-1) - 64*(n-1)^3*a(n-2), a(0) = 1,
 *            a(1) = 4
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   a002895_fixed_width, a002895_rec, a002895_sum
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a002895_fixed_width.h"
#include "../utils/a002895_rec.h"
#include "../utils/a002895_sum.h"

seq_t a(idx_t n) {
    if (a002895_fixed_width()) return a002895_sum(n);
    return a002895_rec(n);
}
