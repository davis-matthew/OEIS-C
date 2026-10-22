/*
 * A002579  Number of integral points in a certain sequence of closed
 *          quadrilaterals.
 * https://oeis.org/A002579
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: (x^5+x^4+x^3+x+1)/((1-x^6)*(1-x)^2), with a(n) the
 *            coefficient of x^n. Let c(m) = [x^m] 1/((1-x^6)(1-x)^2) =
 *            (floor(m/6)+1)*(m+1-3*floor(m/6)). Then a(n) = c(n) + c(n-1) +
 *            c(n-3) + c(n-4) + c(n-5), where c(m) = 0 for m < 0
 * Range:     u8 n <= 23, u16 n <= 395, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   c_val
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/c_val.h"

seq_t a(idx_t n) {
    seq_t result = c_val(n);
    if (n >= 1) result += c_val(n - 1);
    if (n >= 3) result += c_val(n - 3);
    if (n >= 4) result += c_val(n - 4);
    if (n >= 5) result += c_val(n - 5);
    return result;
}
