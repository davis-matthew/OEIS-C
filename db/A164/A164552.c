/*
 * A164552  a(n) = 12*a(n-1)-30*a(n-2) for n > 1; a(0) = 1, a(1) = 10.
 * https://oeis.org/A164552
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 12*a(n-1)-30*a(n-2) for n>1; a(0) = 1, a(1) = 10
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;       /* pointer to cached sequence values */
static idx_t  cache_cap = 0;      /* allocated size of cache   */
static idx_t  computed_up_to = 0; /* index of last computed value */

/* grow the cache so that index 'upto' is valid */
static void grow_cache(idx_t upto)
{
    if (upto < cache_cap)
        return;
    idx_t new_cap = cache_cap ? cache_cap : 4;
    while (new_cap <= upto)
        new_cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp)
        abort();                    /* cannot continue */
    cache = tmp;
    cache_cap = new_cap;
}

/* OEIS A164552 */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)10;

    /* initialise cache with the first two values, if necessary */
    if (cache == NULL) {
        grow_cache(n);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)10;
        computed_up_to = 1;
    } else if (n >= cache_cap) {
        grow_cache(n);
    }

    /* compute missing values up to n using the recurrence */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        cache[i] = cache[i-1] * (seq_t)12 - cache[i-2] * (seq_t)30;
    }
    computed_up_to = n;
    return cache[n];
}
