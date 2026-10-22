/*
 * A000108  Catalan numbers: C(n) = binomial(2n,n)/(n+1) = (2n)!/(n!(n+1)!).
 * https://oeis.org/A000108
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  core, nonn, easy, eigen, nice, changed
 * Formula:   a(n) = 2*(2*n-1)*a(n-1)/(n+1), a(0) = 1
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 19, u64 n <= 36, u128 n <= 69,
 *            big any n
 * Helpers:   cat_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/cat_gcd.h"

seq_t a(idx_t n)
{
    seq_t c = 1;
    idx_t k;
    for (k = 1; k <= n; k++) {
        unsigned long long m = 2ULL * (2ULL * (unsigned long long)k - 1ULL);
        unsigned long long d = (unsigned long long)k + 1ULL;
        /* reduce c by gcd(c, d) so the product never exceeds the result */
        unsigned long long r = (unsigned long long)(c % (seq_t)d);
        unsigned long long g = cat_gcd(d, r);
        c /= (seq_t)g;
        d /= g;
        /* now gcd(c, d) = 1, so d divides m */
        c *= (seq_t)(m / d);
    }
    return c;
}
