/*
 * A035602  Number of points of L1 norm 8 in cubic lattice Z^n.
 * https://oeis.org/A035602
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * Σ_{j=0}^{7} binom(7,j) * binom(n - j + 7, 8); a(0) =
 *            0
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 29, u64 n <= 481, u128 n <= 1000,
 *            big any n
 * Validated: standard, pass, 41 terms through n = 925 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;

    /* binomial coefficients of (1+x)^7 */
    static const seq_t C7[8] = {1, 7, 21, 35, 35, 21, 7, 1};

    seq_t result = 0;

    for (int j = 0; j < 8; ++j) {
        idx_t k = n + 7 - (idx_t)j;   /* n - j + 7  */
        if (k < 8)   /* binom(k,8) is zero for k < 8 */
            continue;
        seq_t term = binomial(k, 8);      /* binom(n - j + 7, 8) */
        term *= C7[j];
        term *= 2;
        result += term;
    }

    return result;
}
