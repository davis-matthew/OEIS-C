/*
 * A180145  Eight rooks and one berserker on a 3 X 3 chessboard. G.f.: (1 -
 *          3*x^2)/(1 - 4*x - 3*x^2 + 6*x^3).
 * https://oeis.org/A180145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 4*a(n-1) + 3*a(n-2) - 6*a(n-3); a(0)=1, a(1)=4, a(2)=16
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 15, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* array holding computed terms   */
static idx_t  cache_size = 0;    /* actual allocated size in elements */
static idx_t  computed_until = 0;/* highest index that has been filled */

/* Initialize cache with the first three values.  */
static void init_cache(void)
{
    cache_size = 3;
    cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
    cache[0] = 1;
    cache[1] = 4;
    cache[2] = 16;
    computed_until = 2;
}

/* Return a(n) using the linear recurrence, memoising all intermediate
   terms to allow repeated calls in any order.  */
seq_t a(idx_t n)
{
    /* Ensure the cache exists. */
    if (!cache)
        init_cache();

    /* If we already have the requested term, just return it. */
    if (n <= computed_until)
        return cache[n];

    /* Grow the array if necessary.  Double the size until it can
       hold index n (indexing starts at 0). */
    if (cache_size <= n) {
        idx_t new_size = cache_size;
        if (new_size == 0)
            new_size = 3;
        while (new_size <= n)
            new_size <<= 1;               /* new_size *= 2; */
        cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        cache_size = new_size;
    }

    /* Compute all missing terms up to n using the recurrence. */
    for (idx_t i = computed_until + 1; i <= n; ++i) {
        cache[i] = 4 * cache[i-1] + 3 * cache[i-2] - 6 * cache[i-3];
    }
    computed_until = n;

    return cache[n];
}
