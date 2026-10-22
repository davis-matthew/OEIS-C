/*
 * A006629  Self-convolution 4th power of A001764, which enumerates ternary
 *          trees.
 * https://oeis.org/A006629
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*binomial(3*n+3,n)/(n+2), computed via a(n) =
 *            a(n-1)*(3n+3)(3n+2)(3n+1)(n+1) / (n*(2n+3)*(2n+2)*(n+2)), a(0) =
 *            1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 48,
 *            big any n
 * Helpers:   a006629_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a006629_gcd.h"

seq_t a(idx_t n)
{
    seq_t r = 1;
    idx_t k;
    for (k = 1; k <= n; k++) {
        unsigned __int128 kk = (unsigned __int128)k;
        unsigned __int128 num = (3 * kk + 3) * (3 * kk + 2) * (3 * kk + 1) * (kk + 1);
        unsigned __int128 den = kk * (2 * kk + 3) * (2 * kk + 2) * (kk + 2);
        unsigned __int128 g = a006629_gcd(num, den);
        num /= g;
        den /= g;
        /* den divides r exactly since r*num/den is an integer and gcd(num,den)=1 */
        r /= (seq_t)(uint64_t)den;
        r *= (seq_t)(uint64_t)num;
    }
    return r;
}
