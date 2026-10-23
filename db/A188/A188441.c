/*
 * A188441  Partial sums of binomial(2n,n)*binomial(3n,n) (A006480).
 * https://oeis.org/A188441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} binomial(2*k,k)*binomial(3*k,k)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 28,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

static seq_t *partial = NULL;
static idx_t partial_len = 0;

seq_t a(idx_t n)
{
    if (n >= partial_len) {
        /* Grow the cache to hold entries up to n */
        seq_t *tmp = (seq_t *)realloc(partial, sizeof(seq_t) * (n + 1));
        if (!tmp) return (seq_t)0;
        partial = tmp;

        for (idx_t k = partial_len; k <= n; ++k) {
            seq_t t1 = binomial((idx_t)(2 * k), k);
            seq_t t2 = binomial((idx_t)(3 * k), k);
            seq_t term = t1 * t2;
            if (k == 0)
                partial[0] = term;
            else
                partial[k] = partial[k - 1] + term;
        }
        partial_len = n + 1;
    }
    return partial[n];
}
