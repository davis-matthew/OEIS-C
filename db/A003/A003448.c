/*
 * A003448  Number of nonequivalent dissections of an n-gon into n-4 polygons
 *          by nonintersecting diagonals rooted at a cell up to rotation and
 *          reflection.
 * https://oeis.org/A003448
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/(2n)) * ( C(n-3,2)*C(2n-6,n-5) + [5|n]*4*j*Cat(j-1)
 *            (j=n/5) + R(n) ), Burnside over D_n, where with f(k,m) =
 *            C(2m-k-1,m-1) - C(2m-k-1,m) and g(k,m) = C(2m-k,m-k): n odd,
 *            m=(n-1)/2: R(n) = n*( g(3,m) + f(2,m) + 3 f(3,m) ); n even,
 *            m=n/2: R(n) = (n/2)*( 2 f(2,m-1) + 4 g(4,m) + 4 f(3,m) +...
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 19, u64 n <= 35, u128 n <= 67,
 *            big any n
 * Helpers:   c3448_bin, c3448_f, c3448_g, c3448_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/c3448_bin.h"
#include "../utils/c3448_f.h"
#include "../utils/c3448_g.h"
#include "../utils/c3448_gcd.h"

seq_t a(idx_t n)
{
    uint64_t D = 4 * (uint64_t)n;
    uint64_t K = (uint64_t)(n - 3) * (uint64_t)(n - 4);
    seq_t X = c3448_bin(2 * n - 6, n - 5);
    uint64_t xr = (uint64_t)(X % (seq_t)D);
    seq_t q = (X / (seq_t)D) * (seq_t)K;
    uint64_t t = xr * K;
    q += (seq_t)(t / D);
    uint64_t rsum = t % D;

    if (n % 5 == 0) {
        idx_t j = n / 5;
        seq_t cat = c3448_bin(2 * j - 2, j - 1) - c3448_bin(2 * j - 2, j);
        seq_t rot2 = (seq_t)(8 * j) * cat;
        q += rot2 / (seq_t)D;
        rsum += (uint64_t)(rot2 % (seq_t)D);
    }

    if (n % 2 == 1) {
        idx_t m = (n - 1) / 2;
        seq_t ot = c3448_g(3, m) + c3448_f(2, m) + 3 * c3448_f(3, m);
        q += ot / 2;
        rsum += (uint64_t)(ot % 2) * 2 * (uint64_t)n;
    } else {
        idx_t m = n / 2;
        seq_t et = 2 * c3448_f(2, m - 1) + 4 * c3448_g(4, m)
                 + 4 * c3448_f(3, m) + 4 * c3448_f(4, m);
        q += et / 4;
        rsum += (uint64_t)(et % 4) * (uint64_t)n;
    }

    q += (seq_t)(rsum / D);
    return q;
}
