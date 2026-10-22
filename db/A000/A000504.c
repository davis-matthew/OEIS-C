/*
 * A000504  S2(j,2j+3) where S2(n,k) is a 2-associated Stirling number of the
 *          second kind.
 * https://oeis.org/A000504
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n+3)!! * n*(n+1)*(10*n^2+15*n+2) / 810, i.e.
 *            2^(n+1)*Gamma(n+5/2)*(n^2+n)*(10*n^2+15*n+2)/(405*sqrt(Pi)); the
 *            divisor 810 is cancelled factor by factor via gcd so no
 *            intermediate exceeds the final value
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 17,
 *            big any n
 * Helpers:   a504_cut, a504_g
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a504_cut.h"
#include "../utils/a504_g.h"

seq_t a(idx_t n) {
    uint64_t r = 810;
    uint64_t nn = (uint64_t)n;
    seq_t res = 1;
    uint64_t f;
    idx_t k;

    f = a504_cut(nn, &r);
    res *= (seq_t)f;
    f = a504_cut(nn + 1, &r);
    res *= (seq_t)f;
    f = a504_cut(10 * nn * nn + 15 * nn + 2, &r);
    res *= (seq_t)f;
    for (k = 3; k <= 2 * nn + 3; k += 2) {
        f = a504_cut((uint64_t)k, &r);
        res *= (seq_t)f;
    }
    return res;
}
