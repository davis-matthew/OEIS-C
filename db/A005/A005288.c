/*
 * A005288  a(n) = C(n,5) + C(n,4) - C(n,3) + 1, n >= 7.
 * https://oeis.org/A005288
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = C(n,5) + C(n,4) - C(n,3) + 1 for n >= 7, a(6) = 3 (the
 *            G.f. 3*x^6 - x^7*(x-2)*(2*x^4-11*x^3+24*x^2-25*x+11)/(x-1)^6 has
 *            a separate initial term)
 * Range:     u8 n <= 9, u16 n <= 25, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Validated: partial, pass, 33 terms through n = 38 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n) {
    if (n == 6) return (seq_t)3;
    /* for n >= 7, C(n,4) >= C(n,3), so the difference stays nonnegative */
    seq_t d = binomial(n, 4) - binomial(n, 3);
    return binomial(n, 5) + d + 1;
}
