/*
 * A105212  a(1) = 668; for n > 1, a(n) = a(n-1) + 1 + sum of distinct prime
 *          factors of a(n-1) that are < a(n-1).
 * https://oeis.org/A105212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 668; for n>1, a(n) = a(n-1) + 1 + sum of distinct prime
 *            factors of a(n-1) that are < a(n-1)
 * Range:     u8 none, u16 n <= 40, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   sum_distinct_prime_factors_less
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/sum_distinct_prime_factors_less.h"

/* cache for computed terms */
static seq_t *cache = NULL;
static idx_t  cached_upto = 0;   /* number of elements stored in cache */

/* compute a(n) */
seq_t a(idx_t n)
{
    /* undefined for n==0, but return 0 to avoid crash */
    if (n == 0) return 0;

    /* allocate cache for the first term if not yet done */
    if (cached_upto == 0) {
        cache = (seq_t*)malloc(sizeof(seq_t) * 1);
        cache[0] = 668;  /* a(1) */
        cached_upto = 1;
    }

    /* extend cache up to n if necessary */
    if (n > cached_upto) {
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * n);
        if (!tmp) {
            /* allocation failure: return 0 (sequence undefined) */
            return 0;
        }
        cache = tmp;

        for (idx_t i = cached_upto; i < n; ++i) {
            seq_t prev = cache[i - 1];              /* a(i) */
            seq_t add  = 1 + sum_distinct_prime_factors_less(prev);
            cache[i] = prev + add;
        }
        cached_upto = n;
    }

    return cache[n - 1];
}
