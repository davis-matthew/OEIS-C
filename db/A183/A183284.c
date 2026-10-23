/*
 * A183284  Half the number of nX3 binary arrays with the number of 1-1
 *          horizontal, vertical, diagonal and antidiagonal adjacencies equal
 *          to the number of 0-0 adjacencies.
 * https://oeis.org/A183284
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = 1/2 * Σ_{x=0}^{4} Σ_{z=0}^{n-2} * C(4, x) * C(n-2, z) *
 *            C(2n-2, (9n-7 - 3x - 8z)/5) * where the summation is taken only
 *            over integer values of y= * (9n-7 - 3x - 8z)/5 that satisfy 0 ≤
 *            y ≤ 2n-2. For n = 1 the * expression degenerates to a(1) = 1. *
 *            * The implementation uses binomial coeff...
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 12, u64 n <= 23, u128 n <= 44,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n) {
    /* Handle the special case n == 1 separately. */
    if (n == 1) {
        seq_t total = 0;
        /* In a 1×3 board the weights are {1,2,1}.  We need
         * 1*x + 2*y = 2 where x is the number of corner cells
         * chosen (≤2) and y is the number of centre cells chosen
         * (≤1).  There are two solutions: (x=0,y=1) and (x=2,y=0). */
        for (idx_t y = 0; y <= 1; ++y) {
            int64_t need = 2 - (int64_t)2 * y;
            if (need < 0 || need > 2) continue;
            idx_t x = (idx_t)need;                /* need will be 0 or 2 */
            seq_t ways = binom(2, x) * binom(1, y);
            total += ways;
        }
        return total / 2;    /* divide by two to account for complements */
    }

    /* For n >= 2 */
    const idx_t two_n_minus_2 = 2 * n - 2;
    const idx_t n_minus_2     = n - 2;
    const idx_t target        = 9 * n - 7;   /* weighted sum that must be matched */

    seq_t total = 0;

    /* Pre‑compute the handful of binomials for the outer loops. */
    seq_t binom4[5];
    for (idx_t x = 0; x <= 4; ++x)
        binom4[x] = binom(4, x);

    /* Loop over the number of chosen weight‑3 cells (corners). */
    for (idx_t x = 0; x <= 4; ++x) {
        seq_t waysX = binom4[x];
        if (waysX == 0) continue;

        /* For each x, iterate over the number of chosen weight‑8 cells
         * (middle rows). */
        for (idx_t z = 0; z <= n_minus_2; ++z) {
            int64_t rem = (int64_t)target - (int64_t)3 * x - (int64_t)8 * z;
            if (rem < 0) break;            /* further z will only decrease rem */
            if (rem % 5 != 0) continue;    /* y would not be integer */

            idx_t y = (idx_t)(rem / 5);
            if (y > two_n_minus_2) continue;

            seq_t waysZ = binom(n_minus_2, z);
            if (waysZ == 0) continue;

            seq_t waysY = binom(two_n_minus_2, y);
            if (waysY == 0) continue;

            /* Multiply the three independent choices. */
            seq_t prod = waysX * waysZ;
            prod = prod * waysY;
            total += prod;
        }
    }

    return total / 2;    /* final division by two */
}
