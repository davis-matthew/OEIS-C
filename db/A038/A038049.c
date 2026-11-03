/*
 * A038049  Number of labeled rooted trees with 2-colored leaves.
 * https://oeis.org/A038049
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, eigen
 * Formula:   a(n) = Sum_{k=0..n} (binomial(n,k) * (n-k)^(n-1))
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 25,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 134 (2026-11-01)
 * Helpers:   binomial, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    if (n == 0) /* OEIS offset starts at 1; undefined for 0, but return 0 */
        return (seq_t)0;

    seq_t result = (seq_t)0;
    seq_t bin     = (seq_t)1; /* C(n,0) */

    for (idx_t j = 0; j <= n; ++j) {
        seq_t pow_val;
        if (j == 0) {                 /* (0)^(n-1) : special handling for 0^0 */
            pow_val = (n == 1) ? (seq_t)1 : (seq_t)0;
        } else {
            pow_val = ipow_idx((seq_t)j, n - 1);
        }
        result += bin * pow_val;        /* term = binom * power */

        /* update binomial for next j: C(n,j+1) = C(n,j) * (n-j)/(j+1)  */
        if (j < n) {
            bin = bin * ((seq_t)(n - j));
            bin = bin / ((seq_t)(j + 1));
        }
    }
    return result;
}
