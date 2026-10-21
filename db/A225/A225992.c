/*
 * A225992  Number of pairs of orthogonal (-3,4) vectors of length 7*n.
 * https://oeis.org/A225992
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (1/2) * Σ_{k=3n}^{4n} binom(7n,k) * 2^k * binom(7n-k,
 *            16n-4k)
 * Range:     u8 none, u16 n <= 1, u32 n <= 2, u64 n <= 4, u128 n <= 9,
 *            big any n
 * Helpers:   binom, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;              /* offset 1;  n>=1 in practice */

    idx_t m = 7 * n;
    idx_t k_start = 3 * n;
    idx_t k_end   = 4 * n;

    seq_t sum = 0;

    for (idx_t k = k_start; k <= k_end; ++k) {
        idx_t t = 16 * n - 4 * k;           /* number of positions with both -3 */
        seq_t term = binom(m, k);
        term *= pow2(k);
        term *= binom(m - k, t);
        sum += term;
    }

    return sum / 2;   /* unordered pairs of vectors */
}
