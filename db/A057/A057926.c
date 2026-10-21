/*
 * A057926  a(0) = 1, a(1) = 3, a(n) = smallest number such that sequence b(n)
 *          = a(n) OR a(n+1) is strictly monotonically increasing.
 * https://oeis.org/A057926
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 60, u32 n <= 60, u64 n <= 60, u128 n <= 60,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A057926 – minimal a(n) so that (a(n-1) | a(n)) is strictly increasing
 *  a(0) = 1, a(1) = 3.
 *  For n ≥ 2, a(n) is the smallest positive integer such that
 *  (a(n-1) | a(n)) > (a(n-2) | a(n-1)).
 *  The function builds the sequence on demand and memoises all values.
 *  It works for any integer size defined by the build system.
 *  Complexity:   O(n²) in the worst case of a naïve search,
 *                but the inner search is bounded by the first bit
 *                that makes the OR exceed the previous one.
 *                For n ≤ 10000 this is far below the time limit.
 */

#include <stddef.h>
#include <stdint.h>

static seq_t *cache = NULL;   /* memoisation array */
static size_t cache_cap = 0;  /* allocated capacity */
static idx_t computed = 0;    /* number of elements filled (0…computed-1) */

/* ensure the cache has space for at least 'needed' elements */
static void ensure_capacity(size_t needed) {
    if (needed <= cache_cap)
        return;
    size_t new_cap = needed;
    if (cache_cap == 0)
        new_cap = 2;
    else
        new_cap = cache_cap * 2;
    if (new_cap < needed)
        new_cap = needed;
    seq_t *new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
    if (cache) {
        /* copy old contents */
        for (size_t i = 0; i < cache_cap; ++i)
            new_cache[i] = cache[i];
        free(cache);
    }
    cache = new_cache;
    cache_cap = new_cap;
}

/* main sequence function */
seq_t a(idx_t n) {
    /* lazily initialise the first two values */
    if (computed == 0) {
        ensure_capacity(2);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        computed = 2;
    }

    /* extend cache up to n */
    if (n + 1 > cache_cap)
        ensure_capacity(n + 1);

    /* compute any missing terms */
    for (idx_t i = computed; i <= n; ++i) {
        if (i == 0) { cache[0] = (seq_t)1; }
        else if (i == 1) { cache[1] = (seq_t)3; }
        else {
            seq_t prev_a = cache[i - 1];
            seq_t prev_b = cache[i - 2] | cache[i - 1];
            seq_t y = (seq_t)1;
            while ((prev_a | y) <= prev_b)
                ++y;
            cache[i] = y;
        }
    }
    computed = (idx_t)n + 1;   /* all terms up to n are now computed */

    return cache[n];
}
