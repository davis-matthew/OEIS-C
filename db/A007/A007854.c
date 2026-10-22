/*
 * A007854  Expansion of 1/(1 - 3*x*C(x)), where C(x) = (1 - sqrt(1 -
 *          4*x))/(2*x) = g.f. for the Catalan numbers A000108.
 * https://oeis.org/A007854
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (9*a(n-1) - 3*A000108(n-1))/2 for n >= 2, a(0) = 1, a(1)
 *            = 3, computed as a(n) = 4*a(n-1) + (a(n-1) - 3*A000108(n-1))/2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 15, u64 n <= 29, u128 n <= 59,
 *            big any n
 * Helpers:   a007854_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a007854_gcd.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)3;

    seq_t c = 1;   /* Catalan C(0) */
    seq_t av = 3;  /* a(1) */
    for (idx_t i = 2; i <= n; i++) {
        uint64_t m = (uint64_t)(i - 1);
        /* C(m) = C(m-1) * 2(2m-1) / (m+1), done without exceeding C(m) */
        uint64_t num = 2 * (2 * m - 1);
        uint64_t den = m + 1;
        uint64_t g = a007854_gcd(num, den);
        num /= g;
        den /= g;
        seq_t d = (seq_t)den;
        if (d == 0) d = 1; /* only reached after overflow in narrow builds */
        c = (c / d) * (seq_t)num;
        /* a(i) = 4*a(i-1) + (a(i-1) - 3*C(i-1))/2 */
        seq_t diff = av - (seq_t)3 * c;
        av = (seq_t)4 * av + diff / 2;
    }
    return av;
}
