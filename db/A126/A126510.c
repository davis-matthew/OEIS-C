/*
 * A126510  Number of base 15 n-digit numbers with adjacent digits differing
 *          by four or less.
 * https://oeis.org/A126510
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A126510: number of base‑15 n‑digit strings in which adjacent digits
   differ by at most 4.

   Recurrence:
      ways(1,d) = 1 for all d = 0…14
      ways(k+1,d) = ∑_{|e−d|≤4} ways(k,e)   (k ≥ 1)
      a(k) = ∑_{d=0}^{14} ways(k,d)
   a(0) = 1. */

#include <stdlib.h>

static seq_t *cache = NULL;          /* a[0…max_computed] */
static size_t cache_len = 0;         /* allocated entries (>=2 after init) */
static size_t max_computed = 0;      /* largest n whose a[n] is known */
static seq_t dp_curr[15];            /* ways for current length */
static seq_t dp_next[15];            /* temporary for next length */

/* initialize cache and dp for first two terms */
static void init_sequence(void)
{
    cache_len = 2;                     /* indices 0 and 1 */
    cache      = (seq_t *)malloc(cache_len * sizeof(seq_t));
    cache[0]   = (seq_t)1;             /* a(0) = 1 */
    cache[1]   = (seq_t)15;            /* a(1) = 15 */

    /* ways for length 1: 1 for each digit 0…14 */
    for (int d = 0; d < 15; ++d)
        dp_curr[d] = (seq_t)1;

    max_computed = 1;
}

/* ensure cache can hold indices up to n */
static void grow_cache(idx_t n)
{
    if ((size_t)n + 1 > cache_len) {
        size_t new_len = (size_t)n + 1;
        cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        cache_len = new_len;
    }
}

seq_t a(idx_t n)
{
    /* a(0) is defined as 1, no further setup required */
    if (n == 0) return (seq_t)1;

    /* first real use of the sequence: allocate structures */
    if (cache == NULL)
        init_sequence();

    /* if requested term already known, return it */
    if ((size_t)n <= max_computed)
        return cache[n];

    /* extend cache to accommodate the new index */
    grow_cache(n);

    /* compute terms from max_computed+1 up to n */
    for (size_t k = max_computed + 1; k <= (size_t)n; ++k) {
        /* compute dp_next from dp_curr */
        for (int d = 0; d < 15; ++d)
            dp_next[d] = (seq_t)0;

        for (int j = 0; j < 15; ++j) {
            seq_t val = dp_curr[j];
            if (val == (seq_t)0) continue;
            /* neighbor digits e with |e−j| ≤ 4 */
            for (int e = j - 4; e <= j + 4; ++e) {
                if (e < 0 || e > 14) continue;
                dp_next[e] += val;
            }
        }

        /* sum to get a[k] */
        seq_t total = (seq_t)0;
        for (int d = 0; d < 15; ++d)
            total += dp_next[d];

        cache[k] = total;

        /* prepare for next iteration */
        for (int d = 0; d < 15; ++d)
            dp_curr[d] = dp_next[d];
    }

    max_computed = (size_t)n;
    return cache[n];
}
