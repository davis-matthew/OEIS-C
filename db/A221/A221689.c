/*
 * A221689  Number of n X 3 arrays of occupancy after each element stays put
 *          or moves to some horizontal or antidiagonal neighbor, without
 *          consecutive moves in the same direction.
 * https://oeis.org/A221689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=6, a(2)=152, a(3)=3112, a(4)=63676, a(5)=1302720; For n>5:
 *            a(n)=24*a(n-1)-69*a(n-2)-71*a(n-3)+2*a(n-4)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Compute A221689, memoized for any call order. */
#include <stdlib.h>

static seq_t *cache = NULL;      /* cache[n-1] stores a(n) */
static idx_t cache_len = 0;      /* current number of cached terms */

/* Ensure the cache contains values up to and including a(n). */
static void ensure_cache(idx_t n)
{
    if (n == 0) return;

    if (cache_len < 5) {
        /* Allocate at least 5 entries and initialize known terms. */
        size_t need = (size_t) (n < 5 ? n : 5);
        if (cache == NULL) {
            cache = (seq_t *)malloc(need * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, need * sizeof(seq_t));
        }
        cache[0] = (seq_t)6;
        if (need > 1) cache[1] = (seq_t)152;
        if (need > 2) cache[2] = (seq_t)3112;
        if (need > 3) cache[3] = (seq_t)63676;
        if (need > 4) cache[4] = (seq_t)1302720;
        cache_len = (idx_t)need;
    } else if (cache_len < n) {
        /* Extend the cache to size n */
        cache = (seq_t *)realloc(cache, (size_t) n * sizeof(seq_t));
    }

    /* Compute missing values using the recurrence */
    for (idx_t i = cache_len; i < n; ++i) {
        cache[i] = 24 * cache[i - 1]
                 - 69 * cache[i - 2]
                 - 71 * cache[i - 3]
                 + 2  * cache[i - 4];
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;    /* undefined for n<1, return 0 */

    ensure_cache(n);
    return cache[n - 1];
}
