/*
 * A000257  Number of rooted bicubic maps: a(n) = (8*n-4)*a(n-1)/(n+2) for n
 *          >= 2, a(0) = a(1) = 1.
 * https://oeis.org/A000257
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = (8*n-4)*a(n-1)/(n+2) for n >= 2, a(0) = a(1) = 1
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 25, u128 n <= 47,
 *            big any n
 * Helpers:   a257_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a257_gcd.h"

seq_t a(idx_t n)
{
    seq_t v = 1;
    idx_t k;
    if (n < 2)
        return v;
    for (k = 2; k <= n; k++) {
        unsigned long long num = 8ULL * (unsigned long long)k - 4ULL;
        unsigned long long den = (unsigned long long)k + 2ULL;
        unsigned long long g = a257_gcd(num, den);
        num /= g;
        den /= g;
        /* den is coprime to num, so den divides a(k-1) exactly */
        v /= (seq_t)den;
        v *= (seq_t)num;
    }
    return v;
}
