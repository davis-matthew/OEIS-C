/*
 * A180274  Integers whose squares are the sums of 24 consecutive squares.
 * https://oeis.org/A180274
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-6) - a(n-12) for n ≥ 13 a(1) = 70, a(2) = 106,
 *            a(3) = 158, a(4) = 182, a(5) = 274, a(6) = 430, a(7) = 650, a(8)
 *            = 1022, a(9) = 1546, a(10) = 1786, a(11) = 2702, a(12) = 4250
 * Range:     u8 n <= 4, u16 n <= 19, u32 n <= 48, u64 n <= 106, u128 n <= 222,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* 1‑indexed array of known terms */
static idx_t  cache_size = 0;        /* actual size of cache (index 0 unused) */

/* Fill cache up to 'upto' */
static void ensure_cache(idx_t upto)
{
    if (cache == NULL) {
        /* allocate space for indices 0..12 (index 0 unused) */
        cache_size = 13;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        /* initialise the first 12 values */
        const seq_t init[12] = {70, 106, 158, 182, 274, 430,
                                650, 1022, 1546, 1786, 2702, 4250};
        for (idx_t i = 1; i <= 12; ++i)
            cache[i] = init[i-1];
    }

    if (upto >= cache_size) {
        /* enlarge the cache to include 'upto' */
        size_t new_size = upto + 1;    /* indices 0..upto */
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        /* compute new terms using the recurrence */
        for (idx_t i = cache_size; i <= upto; ++i)
            cache[i] = 10 * cache[i-6] - cache[i-12];
        cache_size = new_size;
    }
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;          /* undefined for n < 1, but guard guard */
    ensure_cache(n);
    return cache[n];
}
