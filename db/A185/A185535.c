/*
 * A185535  1/6 the number of n X 2 0..5 arrays with every element equal to
 *          exactly one or two of its horizontal and vertical neighbors.
 * https://oeis.org/A185535
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 10, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula implemented:
   a(1)=1, a(2)=11, a(3)=110, a(4)=1155, a(5)=12105
   a(n) = 9*a(n-1) + 19*a(n-2) - 25*a(n-3) - 75*a(n-4) - 125*a(n-5)   (n ≥ 6)
*/

#include <stdlib.h>
#include <stddef.h>

/* External integer types provided by the build system. */
extern seq_t a(idx_t n);

/* Static cache for memoisation */
static seq_t *cache      = NULL;   /* cache[1..computed_max] are valid */
static idx_t  capacity   = 0;
static idx_t  computed_max = 0;

/* Initialise the cache with capacity at least n. */
static void init_cache(idx_t n)
{
    /* start with some room */
    capacity = n > 16 ? n : 16;
    cache = (seq_t *)malloc((capacity + 1) * sizeof(seq_t));
    if (!cache) exit(EXIT_FAILURE);
    for (idx_t i = 0; i <= capacity; ++i) cache[i] = (seq_t)0;

    /* base values */
    cache[1] = (seq_t)1;
    cache[2] = (seq_t)11;
    cache[3] = (seq_t)110;
    cache[4] = (seq_t)1155;
    cache[5] = (seq_t)12105;
    computed_max = 5;
}

/* Ensure that cache contains values up to index n. */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) init_cache(n);

    if (n <= computed_max) return;

    /* Grow cache if needed */
    if (n > capacity) {
        size_t new_cap = capacity * 2;
        if (new_cap < n) new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) exit(EXIT_FAILURE);
        cache = tmp;
        capacity = new_cap;
        /* initialise new entries */
        for (idx_t i = computed_max + 1; i <= capacity; ++i) cache[i] = (seq_t)0;
    }

    /* Compute new terms */
    for (idx_t i = computed_max + 1; i <= n; ++i) {
        if (i <= 5) continue;          /* base values already set */
        seq_t val = (seq_t)9   * cache[i-1];
        val += (seq_t)19  * cache[i-2];
        val -= (seq_t)25  * cache[i-3];
        val -= (seq_t)75  * cache[i-4];
        val -= (seq_t)125 * cache[i-5];
        cache[i] = val;
    }
    computed_max = n;
}

/* Public function required by the specification. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* offset starts at 1; 0 is unused */
    ensure_cache(n);
    return cache[n];
}
