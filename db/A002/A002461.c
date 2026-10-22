/*
 * A002461  Coefficients of Legendre polynomials.
 * https://oeis.org/A002461
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(2) = 1; for n >= 3 with k = n-1, a(n) = (odd part of
 *            binomial(2k,k)) * 2^t(k), where t(k) is the number of trailing 1
 *            bits of k (equivalently n*|A002596(n)|)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 19, u64 n <= 28, u128 n <= 28,
 *            big any n
 * Helpers:   a002461_gcd_loc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a002461_gcd_loc.h"

seq_t a(idx_t n) {
    if (n == 2) return 1;
    idx_t k = n - 1;
    /* O_j = odd part of C(2j,j); O_j = O_{j-1} * (2j-1) / oddpart(j), nondecreasing */
    seq_t O = 1;
    idx_t j;
    for (j = 1; j <= k; j++) {
        idx_t d = j;
        while (!(d & 1)) d >>= 1;
        idx_t m = 2 * j - 1;
        idx_t g = a002461_gcd_loc(m, d);
        m /= g;
        d /= g;
        O = O / d * m;
    }
    idx_t t = k;
    while (t & 1) {
        O *= 2;
        t >>= 1;
    }
    return O;
}
