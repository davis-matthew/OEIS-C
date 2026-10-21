/*
 * A087621  (1,1) entry of powers of the orthogonal design shown below.
 * https://oeis.org/A087621
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = 4*a(n-2) - 64*a(n-4) for n ≥ 4 with a(0)=1, a(1)=-1,
 *            a(2)=2, a(3)=4 (offset 1)
 * Range:     i8 n <= 5, i16 n <= 12, i32 n <= 21, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

/* Initialize the first four terms needed by the recurrence. */
static void init_cache(void)
{
    if (cache_size == 0) {
        cache_size = 4;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)-1;
        cache[2] = (seq_t)2;
        cache[3] = (seq_t)4;
    }
}

/* Expand the cache up to index n (inclusive) using the recurrence. */
static void expand_cache(idx_t n)
{
    if (n + 1 <= cache_size) return;
    size_t new_size = (size_t)(n + 1);
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) exit(1);
    cache = new_cache;
    for (idx_t i = cache_size; i <= n; ++i) {
        cache[i] = (seq_t)4 * cache[i-2] - (seq_t)64 * cache[i-4];
    }
    cache_size = new_size;
}

seq_t a(idx_t n)
{
    init_cache();
    if (n == 0) return (seq_t)0;   /* not part of OEIS offset */
    expand_cache(n);
    return cache[n];
}
