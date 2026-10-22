/*
 * A000139  a(n) = 2*(3*n)! / ((2*n+1)!*(n+1)!).
 * https://oeis.org/A000139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 2*(3*n)! / ((2*n+1)!*(n+1)!), computed via a(n) = a(n-1)
 *            * 3*(3*n-1)*(3*n-2) / (2*(2*n+1)*(n+1)), a(0) = 2
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 15, u64 n <= 27, u128 n <= 52,
 *            big any n
 * Helpers:   a139_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a139_gcd.h"

seq_t a(idx_t n)
{
    seq_t v = 2;
    idx_t k;
    for (k = 1; k <= n; k++) {
        unsigned long long num = 3ULL * (3ULL * k - 1) * (3ULL * k - 2);
        unsigned long long den = 2ULL * (2ULL * k + 1) * (k + 1);
        unsigned long long r = (unsigned long long)(v % (seq_t)den);
        unsigned long long g = a139_gcd(r, den);   /* gcd(v, den) */
        unsigned long long d2;
        v /= (seq_t)g;
        d2 = den / g;                               /* coprime to v, so divides num */
        v *= (seq_t)(num / d2);
    }
    return v;
}
