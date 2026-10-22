/*
 * A002057  Fourth convolution of Catalan numbers: a(n) =
 *          4*binomial(2*n+3,n)/(n+4).
 * https://oeis.org/A002057
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(n-1) * 2*(2*n+3)*(n+1) / (n*(n+4)), a(0) = 1 (i.e. a(n)
 *            = 4*binomial(2*n+3,n)/(n+4))
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 17, u64 n <= 33, u128 n <= 66,
 *            big any n
 * Helpers:   a002057_gcd_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a002057_gcd_u.h"

seq_t a(idx_t n) {
    seq_t r = (seq_t)1;
    idx_t i;
    for (i = 1; i <= n; i++) {
        uint64_t num = (uint64_t)2 * (2 * (uint64_t)i + 3) * ((uint64_t)i + 1);
        uint64_t den = (uint64_t)i * ((uint64_t)i + 4);
        uint64_t g = a002057_gcd_u(num, den);
        num /= g;
        den /= g;
        /* num and den are coprime and r*num/den is an integer, so den divides r */
        r = r / (seq_t)den;
        r = r * (seq_t)num;
    }
    return r;
}
