/*
 * A076975  a(1) = 1, a(2n) = smallest composite number > (2n-1)-th partial
 *          sum of the sequence itself and a(2n+1) = smallest prime > 2n-th
 *          partial sum of the sequence.
 * https://oeis.org/A076975
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; For n > 1: if n is even: a(n) = smallest composite >
 *            sum_{i=1}^{n-1} a(i) if n is odd : a(n) = smallest prime >
 *            sum_{i=1}^{n-1} a(i)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 16, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

/* Cached sequence values and partial sum.
   The cache grows dynamically on first access. */
static seq_t *cache = NULL;           /* array of computed a(i) */
static idx_t  cache_count = 0;        /* number of terms currently cached   */
static seq_t  partial_sum = 0;        /* sum of first cache_count terms     */

/* Return the smallest prime > start (start is a seq_t). */
static inline seq_t next_prime_from(seq_t start)
{
    seq_t v = start + (seq_t)1;
    while (!is_prime_big(v))
        ++v;
    return v;
}

/* Return the smallest composite > start. */
static inline seq_t next_composite_from(seq_t start)
{
    seq_t v = start + (seq_t)1;
    while (v <= (seq_t)1 || is_prime_big(v))
        ++v;
    return v;
}

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined, return 0 for safety */
        return (seq_t)0;

    /* compute terms up to n if necessary */
    if (n > cache_count) {
        size_t newsize = (size_t)n;
        cache = (seq_t *)realloc(cache, newsize * sizeof(seq_t));
        /* extend cache (partial_sum already holds sum up to cache_count) */
        for (idx_t i = cache_count + (idx_t)1; i <= n; ++i) {
            seq_t term;
            if (i == (idx_t)1) {
                term = (seq_t)1;
            } else if ((i & (idx_t)1) == (idx_t)0) {   /* even index */
                term = next_composite_from(partial_sum);
            } else {                                 /* odd index */
                term = next_prime_from(partial_sum);
            }
            cache[i - (idx_t)1] = term;
            partial_sum += term;
        }
        cache_count = n;
    }
    return cache[n - (idx_t)1];
}
