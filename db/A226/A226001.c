/*
 * A226001  Number of pairs of orthogonal (-3,7) vectors of length 10*n.
 * https://oeis.org/A226001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2) * Σ_{m=0}^{n} binom(10n, 7m) * binom(10n-7m, 3n-3m)
 *            * 2^{7n-4m} (Sequences counts unordered pairs of orthogonal
 *            (-3,7) vectors of length 10*n)
 * Range:     u8 none, u16 n <= 1, u32 n <= 1, u64 n <= 3, u128 n <= 6,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;        /* offset is 1 */

    seq_t sum = 0;
    idx_t total = 10 * n;

    for (idx_t m = 0; m <= n; ++m)
    {
        idx_t x = 7 * m;                  /* count of weight 9 (= (-3,-3)) */
        idx_t w = 3 * (n - m);            /* count of weight 49 (= (7,7)) */
        idx_t t = 7 * n - 4 * m;          /* count of weight -21 (= (-3,7) or (7,-3)) */

        seq_t term = binom(total, x);
        term *= binom(total - x, w);
        term *= pow2(t);

        sum += term;
    }

    return sum / 2;
}
