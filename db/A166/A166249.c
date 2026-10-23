/*
 * A166249  a(n) = 2*a(n-1) - a(n-3) + 2*a(n-4) for n>3, a(0)=a(1)=1, a(2)=0,
 *          a(3)=2.
 * https://oeis.org/A166249
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n‑1) - a(n‑3) + 2*a(n‑4) with initial values a(0)=1,
 *            a(1)=1, a(2)=0, a(3)=2
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 33, u64 n <= 65, u128 n <= 129,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;     /* memoized values of the sequence     */
static size_t cache_size = 0;   /* number of elements currently stored */

/* Expand the cache so that it contains terms up to index 'up_to' (inclusive). */
static void ensure_cache(size_t up_to)
{
    if (up_to < cache_size)
        return; /* already available */

    size_t new_size = up_to + 1;
    if (new_size < 4)
        new_size = 4;            /* at least room for the four base terms */

    /* Allocate a new array of the required size. */
    seq_t *new_cache = (seq_t *)malloc(new_size * sizeof(seq_t));
    if (!new_cache)
        abort(); /* Allocation failure */

    /* Copy existing values into the new array. */
    for (size_t i = 0; i < cache_size && i < new_size; ++i)
        new_cache[i] = cache[i];

    /* Free the old array if it existed. */
    if (cache)
        free(cache);

    cache = new_cache;
    cache_size = new_size;

    /* Initialise the base values if this is the first allocation or a
       reallocation that now contains the first four terms.  We initialise
       only once. */
    if (cache_size >= 4 && cache[0] == (seq_t)0 && cache[1] == (seq_t)0 &&
        cache[2] == (seq_t)0 && cache[3] == (seq_t)0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)2;
    }

    /* Compute any new terms using the recurrence. */
    for (size_t i = 4; i < cache_size; ++i) {
        cache[i] = (seq_t)((seq_t)2 * cache[i - 1]
                         - cache[i - 3]
                         + (seq_t)2 * cache[i - 4]);
    }
}

seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    ensure_cache(idx);
    return cache[idx];
}
