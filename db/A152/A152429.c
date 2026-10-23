/*
 * A152429  a(n) = (11^n + 5^n)/2.
 * https://oeis.org/A152429
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = 16*a(n-1) - 55*a(n-2); a(0)=1, a(1)=8
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* directly equivalent to a(n) = (11^n + 5^n)/2 */

#include <stdlib.h>
#include <stdbool.h>

static seq_t *cache = NULL;  /* memoised sequence values          */
static idx_t cached_up_to = 0;  /* highest n that has a valid entry */

/* Make sure the cache has room for indices 0..n.
   Does NOT mark values beyond cached_up_to as valid. */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* First allocation */
        size_t sz = (size_t)(n + 1);
        cache = (seq_t *)malloc(sz * sizeof(seq_t));
        if (cache == NULL) abort();

        /* initialise all elements to 0 */
        for (idx_t i = 0; i <= n; ++i) cache[i] = 0;

        /* set the base cases */
        cache[0] = (seq_t)1;
        if (n >= 1) cache[1] = (seq_t)8;

        /* mark the highest valid index */
        cached_up_to = (n >= 1) ? (idx_t)1 : (idx_t)0;
    } else if (n > cached_up_to) {
        /* grow the cache; keep existing values */
        size_t new_size = (size_t)(n + 1);
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (tmp == NULL) abort();
        cache = tmp;

        /* initialise the newly added slots to 0 */
        for (idx_t i = cached_up_to + 1; i <= n; ++i) cache[i] = 0;
        /* do NOT change cached_up_to here – values are not yet computed */
    }
}

/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)8;

    ensure_cache(n);

    /* compute any missing values from cached_up_to+1 up to n */
    for (idx_t i = cached_up_to + 1; i <= n; ++i) {
        /* recurrence: a(i) = 16*a(i-1) - 55*a(i-2) */
        cache[i] = ((seq_t)16 * cache[i - 1]) - ((seq_t)55 * cache[i - 2]);
    }
    cached_up_to = n;
    return cache[n];
}
