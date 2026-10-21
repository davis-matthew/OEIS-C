/*
 * A097770  Pell equation solutions (12*b(n))^2 - 145*a(n)^2 = -1 with b(n) =
 *          A097769(n), n >= 0.
 * https://oeis.org/A097770
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 578*a(n-1) - a(n-2), a(0) = 1, a(1) = 577
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 3, u64 n <= 6, u128 n <= 13,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence of order 2, computed with memoised iterative evaluation. */

#include <stdlib.h>

static seq_t *cache = NULL;              /* dynamic array of computed terms */
static size_t cache_size = 0;            /* allocated size of cache array   */
static idx_t  computed_up_to = 0;        /* one past the largest index computed */

/* Ensure that cache has room for indices up to n and compute any missing terms. */
static void ensure_computed(idx_t n)
{
    if (computed_up_to > n) return;     /* already computed */

    /* Allocate or grow the array to at least n+1 entries. */
    size_t needed = (size_t)n + 1;
    if (needed > cache_size) {
        size_t new_size = cache_size ? cache_size << 1 : 8;
        if (new_size < needed) new_size = needed;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!new_cache) abort();        /* allocation failure */
        cache = new_cache;
        cache_size = new_size;
    }

    /* Compute terms from computed_up_to up to n, inclusive. */
    for (idx_t i = computed_up_to; i <= n; ++i) {
        seq_t val;
        if (i == 0) {
            val = 1;
        } else if (i == 1) {
            val = 577;
        } else {
            val = ((seq_t)578 * cache[i-1]) - cache[i-2];
        }
        cache[i] = val;
    }
    computed_up_to = n + 1;
}

seq_t a(idx_t n)
{
    ensure_computed(n);
    return cache[n];
}
