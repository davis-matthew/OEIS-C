/*
 * A000245  a(n) = 3*(2*n)!/((n+2)!*(n-1)!).
 * https://oeis.org/A000245
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = a(n-1) * (2*n)*(2*n-1) / ((n+2)*(n-1)) for n >= 2, a(0) =
 *            0, a(1) = 1 (equivalently a(n) = 3*(2*n)!/((n+2)!*(n-1)!))
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   a245_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a245_gcd.h"

static seq_t a245_val;
static idx_t a245_idx = 0; /* 0 = cache empty */

seq_t a(idx_t n)
{
    seq_t v;
    idx_t k;

    if (n == 0) return (seq_t)0;
    if (n == 1) return (seq_t)1;

    if (a245_idx >= 1 && a245_idx <= n) {
        v = a245_val;
        k = a245_idx;
    } else {
        v = (seq_t)1;
        k = 1;
    }

    while (k < n) {
        unsigned long long m = (unsigned long long)(k + 1);
        unsigned long long num = (2ULL * m) * (2ULL * m - 1ULL);
        unsigned long long den = (m + 2ULL) * (m - 1ULL);
        unsigned long long g, r;

        /* reduce num/den first */
        g = a245_gcd(num, den);
        num /= g;
        den /= g;

        /* cancel remaining denominator against the current value */
        if (den > 1) {
            r = (unsigned long long)(v % (seq_t)den);
            g = a245_gcd(den, r);
            v /= (seq_t)g;
            den /= g;
        }
        /* den now divides num exactly */
        num /= den;
        v *= (seq_t)num;
        k++;
    }

    a245_val = v;
    a245_idx = n;
    return v;
}
