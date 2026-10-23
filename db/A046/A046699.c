/*
 * A046699  a(1) = a(2) = 1, a(n) = a(n - a(n-1)) + a(n-1 - a(n-2)) if n > 2.
 * https://oeis.org/A046699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 503, u16 n <= 9543, u32 n <= 9543, u64 n <= 9543,
 *            u128 n <= 9543, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A046699:
   a(1) = a(2) = 1,
   a(n) = a(n - a(n-1)) + a(n-1 - a(n-2))   for n > 2
   (indexing starts at 1) */
#include <stdlib.h>

static seq_t *cache = NULL;   /* array of computed values (1‑based) */
static size_t cache_cap = 0;  /* current capacity of cache */
static idx_t  computed_up_to = 0; /* largest index that has been computed */

/* Compute a(n).  The function is memoised and works for any order of calls. */
seq_t a(idx_t n)
{
    /* Ensure the cache can hold index n.  Allocate at least 3 slots to
       safely store a(1) and a(2).  Use size_t for the new capacity. */
    if (n >= cache_cap) {
        size_t new_cap = (size_t)n + 1;  /* index 0 .. n  */
        if (new_cap < 3) new_cap = 3;    /* need indices 1 and 2 */
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Initialise base cases if not already done. */
    if (computed_up_to < 2) {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)1;
        computed_up_to = 2;
    }

    /* Compute any missing values up to n. */
    for (idx_t i = computed_up_to + 1; i <= n; ++i) {
        idx_t idx1 = i - (idx_t)cache[i - 1];          /* i - a(i-1)   */
        idx_t idx2 = (i - 1) - (idx_t)cache[i - 2];    /* i-1 - a(i-2) */
        cache[i] = cache[idx1] + cache[idx2];
    }
    computed_up_to = n;

    return cache[n];
}
