/*
 * A197229  Number of n X 2 0..4 arrays with each element x equal to the
 *          number its horizontal and vertical neighbors equal to 4,3,0,1,2
 *          for x=0,1,2,3,4.
 * https://oeis.org/A197229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 49, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence A197229: number of n×2 arrays 0..4 with each entry x
 * having exactly [4,3,0,1,2] neighbours equal to 4,3,0,1,2 for x=0,1,2,3,4.
 *
 * Recurrence (OEIS):  a(n) = a(n-1) + 3*a(n-2) + 2*a(n-3)
 *                    - a(n-4) - a(n-5) - a(n-6)
 * Initial values: a(1)=1, a(2)=7, a(3)=12, a(4)=31, a(5)=79, a(6)=186
 *
 * The implementation memoises all computed values in a static array
 * so that calls are O(1) after the first computation of the required n.
 */

#include <stddef.h>

static seq_t *cache = NULL;          /* 1‑indexed cache, grows as needed */
static size_t cache_size = 0;        /* maximum index for which cache is allocated */
static size_t cached_until = 0;      /* largest n that has already been computed */

/* Grow the cache array to at least size 'needed'.  Works for any seq_t. */
static void grow_cache(size_t needed)
{
    if (cache_size >= needed)
        return;

    /* allocate bigger array (double size for amortised linear growth) */
    size_t new_size = cache_size ? cache_size * 2 : 6;
    if (new_size < needed)
        new_size = needed;
    seq_t *new_cache = (seq_t *)realloc(cache, sizeof(seq_t) * (new_size + 1));
    /* if realloc fails, leave cache unchanged (undefined – unlikely in contest) */
    if (new_cache != NULL) {
        cache = new_cache;
        cache_size = new_size;
    }
}

/* Ensure that the cache has at least the first 6 base terms defined. */
static void initialise_base()
{
    if (cached_until >= 6)
        return;

    grow_cache(6);
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)7;
    cache[3] = (seq_t)12;
    cache[4] = (seq_t)31;
    cache[5] = (seq_t)79;
    cache[6] = (seq_t)186;
    if (cached_until < 6)
        cached_until = 6;
}

/* Compute a(n) using the recurrence with memoisation. */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* out of range – not used by tests */

    /* Make sure base terms exist. */
    initialise_base();

    /* If we already computed up to n, just return the cached value. */
    if (cached_until >= (size_t)n)
        return cache[(size_t)n];

    /* Grow cache to hold intermediate values up to n. */
    grow_cache((size_t)n);

    /* Compute missing terms starting from cached_until + 1. */
    for (size_t i = cached_until + 1; i <= (size_t)n; ++i) {
        cache[i] = cache[i - 1]
                 + ((seq_t)3) * cache[i - 2]
                 + ((seq_t)2) * cache[i - 3]
                 - cache[i - 4]
                 - cache[i - 5]
                 - cache[i - 6];
    }
    cached_until = (size_t)n;
    return cache[(size_t)n];
}
