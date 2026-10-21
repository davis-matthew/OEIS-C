/*
 * A052986  Expansion of ( 1-2*x ) / ( (x-1)*(2*x^2+3*x-1) ).
 * https://oeis.org/A052986
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 70,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A052986
 * Recurrence:  a(0) = 1,  a(1) = 2,
 *              a(n+2) = 2*a(n) + 3*a(n+1) - 1   (for n ≥ 0)
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* dynamic cache for computed terms   */
static idx_t  cache_size = 0;    /* number of valid entries in cache  */

static void ensure_cache(idx_t n)
{
    if (n < cache_size) return;         /* already computed */

    /* new cache size needed: one more for index n          */
    size_t new_cap = (size_t)n + 1;
    seq_t *new_arr;

    if (cache == NULL) {
        new_arr = (seq_t*)malloc(new_cap * sizeof(seq_t));
    } else {
        new_arr = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    }
    if (!new_arr) return;               /* allocation failed – leave cache unchanged */

    /* initialise or extend entries */
    if (cache == NULL) {                /* first allocation */
        new_arr[0] = (seq_t)1;
        if (new_cap > 1) new_arr[1] = (seq_t)2;
        cache_size = new_cap;
        cache = new_arr;
        return;
    }

    /* cache existed: preserve the old ok contents */
    if (cache_size == 1 && new_cap > 1) {
        new_arr[1] = (seq_t)2;          /* set the second term if it was missing */
    }

    /* compute new terms using recurrence */
    size_t start = cache_size;
    if (start < 2) start = 2;           /* if cache_size was 1, need to compute a[2], etc. */

    for (size_t i = start; i < new_cap; ++i) {
        seq_t prev2 = new_arr[i-2];
        seq_t prev1 = new_arr[i-1];
        seq_t val = (seq_t)2 * prev2 + (seq_t)3 * prev1 - (seq_t)1;
        new_arr[i] = val;
    }

    cache = new_arr;
    cache_size = new_cap;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
