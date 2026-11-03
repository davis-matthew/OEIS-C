/*
 * A151637  Number of permutations of 3 indistinguishable copies of 1..n with
 *          exactly 7 adjacent element pairs in decreasing order.
 * https://oeis.org/A151637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{j=0..9} (-1)^(j+1)*binomial(3*n+1,
 *            9-j)*(binomial(j+1, 3))^n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 193 (2026-11-01)
 * Helpers:   binomial, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/binomial.h"
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t res = 0;
    for (idx_t j = 0; j <= 9; ++j) {
        seq_t base = binomial(j + 1, 3);            /* (j+1 choose 3) */
        if (base == 0) continue;                   /* skips j=0,1,2 */
        seq_t powv = ipow(base, (seq_t)n);       /* base^n */
        seq_t comb = binomial(3 * n + 1, 9 - j);    /* (3n+1 choose 9-j) */
        seq_t term = powv * comb;
        if (j & 1)        /* j odd → positive sign */
            res += term;
        else              /* j even → negative sign */
            res -= term;
    }
    return res;
}
