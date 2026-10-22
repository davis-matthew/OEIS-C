/*
 * A000740  Number of 2n-bead balanced binary necklaces of fundamental period
 *          2n, equivalent to reversed complement; also Dirichlet convolution
 *          of b_n=2^(n-1) with mu(n); also number of components of Mandelbrot
 *          set corresponding to Julia sets with an attractive n-cycle.
 * https://oeis.org/A000740
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = Sum_{d|n} mu(n/d)*2^(d-1)
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   a000740_add, a000740_mu, a000740_pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/a000740_add.h"
#include "../utils/a000740_mu.h"
#include "../utils/a000740_pow2.h"

seq_t a(idx_t n) {
    seq_t pos = 0, neg = 0;
    idx_t d;
    for (d = 1; d * d <= n; d++) {
        if (n % d == 0) {
            idx_t other = n / d;
            a000740_add(d, other, &pos, &neg);
            if (other != d) a000740_add(other, d, &pos, &neg);
        }
    }
    return pos - neg;
}
