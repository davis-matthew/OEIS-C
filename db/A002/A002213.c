/*
 * A002213  Number of tree-like polyhexes rooted at a hexagon and containing n
 *          hexagons.
 * https://oeis.org/A002213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: x + x*U(x) + (3/2)*x*U(x)^2 + (1/2)*x*U(x^2) +
 *            (1/3)*x*U(x)^3 + (2/3)*x*U(x^3), U(x) = (1 - 3*x -
 *            sqrt((1-x)*(1-5*x)))/(2*x) = Sum_{k>=1} A002212(k) x^k. With m =
 *            n-1: a(n) = [m=0] + u(m) + (3*c2(m) + u(m/2))/2 + (c3(m) +
 *            2*u(m/3))/3, where c2(m) = u(m+1) - 3*u(m) - [m=0], c3(m) =
 *            c2(m+1) -...
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 16, u64 n <= 31, u128 n <= 59,
 *            big any n
 * Helpers:   a2213_big, a2213_fixed, a2213_is_fixed
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/a2213_big.h"
#include "../utils/a2213_fixed.h"
#include "../utils/a2213_is_fixed.h"

seq_t a(idx_t n) {
    if (n < 1) return 0;
    idx_t m = n - 1;
    if (a2213_is_fixed()) return a2213_fixed(m);
    return a2213_big(m);
}
