/*
 * A000309  Number of rooted planar bridgeless cubic maps with 2n nodes.
 * https://oeis.org/A000309
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = 3*(3*n-1)*(3*n-2)*a(n-1) / ((n+1)*(2*n+1)), a(0) = 1
 *            (equivalently a(n) = 2^n*(3*n)!/((n+1)!*(2*n+1)!))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 38,
 *            big any n
 * Helpers:   a309_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a309_gcd.h"

seq_t a(idx_t n)
{
    seq_t v = 1;
    idx_t k;
    for (k = 1; k <= n; k++) {
        unsigned long long num = 3ULL * (3ULL * k - 1) * (3ULL * k - 2);
        unsigned long long den = (unsigned long long)(k + 1) * (2ULL * k + 1);
        unsigned long long g = a309_gcd(num, den);
        num /= g;
        den /= g;
        /* den is coprime to num, so it divides a(k-1) exactly */
        v /= (seq_t)den;
        v *= (seq_t)num;
    }
    return v;
}
