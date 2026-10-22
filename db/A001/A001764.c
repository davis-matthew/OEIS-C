/*
 * A001764  a(n) = binomial(3*n,n)/(2*n+1) (enumerates ternary trees and also
 *          noncrossing trees).
 * https://oeis.org/A001764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice, core
 * Formula:   a(n) = binomial(3*n,n)/(2*n+1), computed by the recurrence a(n)
 *            = a(n-1) * 3*(3n-1)*(3n-2) / (2n*(2n+1)), a(0) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 26, u128 n <= 50,
 *            big any n
 * Helpers:   a001764_gcd_u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a001764_gcd_u.h"

seq_t a(idx_t n)
{
    seq_t r = 1;
    idx_t k;
    for (k = 1; k <= n; k++) {
        uint64_t num = (uint64_t)3 * (3 * k - 1) * (3 * k - 2);
        uint64_t den = (uint64_t)2 * k * (2 * k + 1);
        uint64_t g = a001764_gcd_u(num, den);
        num /= g;
        den /= g;
        /* num/den is in lowest terms and r*num/den is an integer, so den | r */
        r /= (seq_t)den;
        r *= (seq_t)num;
    }
    return r;
}
