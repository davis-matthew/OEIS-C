/*
 * A042044  Numerators of continued fraction convergents to sqrt(546).
 * https://oeis.org/A042044
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy, less
 * Formula:   a(n) = numerator of the n-th convergent to sqrt(546) The
 *            continued fraction of sqrt(546) begins [23; 2,1,2,1,2,46,
 *            2,1,2,1,2,46, ...] where after the initial 23 the partial
 *            quotients repeat every six terms: 2,1,2,1,2,46. A convergent
 *            (p_k,q_k) satisfies p_k = a_k * p_{k-1} + p_{k-2}, q_k = a_k
 *            *...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 17, u64 n <= 35, u128 n <= 71,
 *            big any n
 * Helpers:   cf_quotient
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/cf_quotient.h"

static seq_t *cache = NULL;      /* memoised numerators */
static size_t cache_len = 0;     /* number of stored terms */

seq_t a(idx_t n)
{
    /* initialise cache for the very first term */
    if (cache_len == 0)
    {
        cache_len = 1;
        cache = (seq_t *)realloc(NULL, cache_len * sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = cf_quotient(0);          /* 23 */
    }

    /* if we already have the requested term, return it */
    if ((size_t)n < cache_len)
        return cache[(size_t)n];

    /* Set up the previous two numerator values needed for recurrence.
       For the first new term (index 1) the fictitious previous term
       p_{-1} equals 1.  For larger indices we retrieve the two
       most recent stored numerators. */
    seq_t prev2, prev1;
    if (cache_len == 1)          /* we only have p_0 stored */
    {
        prev2 = (seq_t)1;        /* p_{-1} */
        prev1 = cache[0];        /* p_0 */
    }
    else                          /* cache_len >= 2 */
    {
        prev2 = cache[cache_len - 2];  /* p_{n-1} */
        prev1 = cache[cache_len - 1];  /* p_n   */
    }

    /* Grow the cache to hold terms up to n */
    size_t new_len = (size_t)n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);
    cache = tmp;

    /* Generate missing terms using the recurrence relation */
    for (size_t i = cache_len; i < new_len; ++i)
    {
        seq_t q = cf_quotient(i);           /* partial quotient a_i */
        seq_t cur = q * prev1 + prev2;     /* p_i = a_i * p_{i-1} + p_{i-2} */
        cache[i] = cur;
        prev2 = prev1;
        prev1 = cur;
    }
    cache_len = new_len;

    return cache[(size_t)n];
}
