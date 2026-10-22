/*
 * A000168  a(n) = 2*3^n*(2*n)!/(n!*(n+2)!).
 * https://oeis.org/A000168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = 2*3^n*(2*n)!/(n!*(n+2)!), computed via (n+2)*a(n) =
 *            6*(2*n-1)*a(n-1), a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 39,
 *            big any n
 * Helpers:   a168_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a168_gcd.h"

seq_t a(idx_t n)
{
    seq_t v = 1;
    idx_t k;
    for (k = 1; k <= n; k++) {
        unsigned long long q = 6ULL * (2ULL * (unsigned long long)k - 1ULL);
        unsigned long long d = (unsigned long long)k + 2ULL;
        unsigned long long g = a168_gcd(q, d);
        q /= g;
        d /= g;
        /* gcd(q, d) = 1 and d | v*q, hence d | v */
        v /= d;
        v *= q;
    }
    return v;
}
