/*
 * A007818  Maximal number of bonds joining n nodes in simple cubic lattice.
 * https://oeis.org/A007818
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n - A193416(n)/2. The optimal polycube is a quasi-cube
 *            a x b x c (a<=b<=c<=a+1) of the largest volume m <= n, plus r =
 *            n-m cubes placed as an optimal square layer on its largest face,
 *            so a(n) = 3m - (ab+bc+ca) + r + 2r - ceil(2*sqrt(r))
 * Range:     u8 n <= 108, u16 n <= 500, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   a007818_ceil2sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a007818_ceil2sqrt.h"

seq_t a(idx_t n) {
    uint64_t N = (uint64_t)n;
    uint64_t l = 1;
    while ((l + 1) * (l + 1) * (l + 1) <= N) l++;
    uint64_t A = l, B = l, C = l;
    if (l * (l + 1) * (l + 1) <= N) { B = l + 1; C = l + 1; }
    else if (l * l * (l + 1) <= N) { C = l + 1; }
    uint64_t m = A * B * C;
    uint64_t r = N - m;

    seq_t sm = (seq_t)m;
    seq_t surf = (seq_t)(A * B) + (seq_t)(B * C) + (seq_t)(A * C);
    seq_t bonds = sm * 3 - surf;
    if (r > 0) {
        uint64_t t = a007818_ceil2sqrt(r);
        bonds += (seq_t)r;
        bonds += (seq_t)(2 * r - t);
    }
    return bonds;
}
