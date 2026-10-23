/*
 * A117189  Binomial transform of the tribonacci sequence A000073 (shifted
 *          left twice).
 * https://oeis.org/A117189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=2, a(2)=5, a(n)=4*a(n-1)-4*a(n-2)+2*a(n-3) for n>2
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 21, u64 n <= 42, u128 n <= 85,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t *cache = NULL;      /* memoized array of sequence values */
static idx_t cache_size = 0;     /* number of elements currently stored */

/* Return a(n) for any n >= 0.
   The function is thread‑unsafe but satisfies the required semantics
   for any order of calls. */
seq_t a(idx_t n)
{
    /* If the requested value is already cached, return it. */
    if (n < cache_size) {
        return cache[n];
    }

    /* Need to grow the cache to accommodate n. */
    size_t new_size = (size_t)n + 1;          /* new array length */

    /* Allocate new buffer. */
    seq_t *tmp = alloc_seq_array(new_size);
    if (!tmp) abort();                       /* allocation failure */

    /* Copy existing values into the new buffer. */
    if (cache) {
        size_t copy_len = cache_size < new_size ? cache_size : new_size;
        for (size_t i = 0; i < copy_len; ++i) {
            tmp[i] = cache[i];
        }
        /* Release the old buffer. */
        free_seq_array(cache);
    }

    /* Install the new buffer and size. */
    cache = tmp;
    cache_size = (idx_t)new_size;

    /* Initialise base values. */
    if (cache_size > 0) cache[0] = (seq_t)1;
    if (cache_size > 1) cache[1] = (seq_t)2;
    if (cache_size > 2) cache[2] = (seq_t)5;

    /* Compute missing terms using the recurrence. */
    for (size_t i = 3; i < cache_size; ++i) {
        cache[i] = ((seq_t)4) * cache[i - 1]
                 - ((seq_t)4) * cache[i - 2]
                 + ((seq_t)2) * cache[i - 3];
    }

    return cache[n];
}
