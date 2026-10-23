/*
 * A020709  Pisot sequence E(4,10).
 * https://oeis.org/A020709
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + 2*a(n-3) for n >= 3 initial values:
 *            a(0)=4, a(1)=10, a(2)=25 offset: 0
 * Range:     u8 n <= 4, u16 n <= 10, u32 n <= 22, u64 n <= 46, u128 n <= 94,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

static seq_t *seq_cache = NULL;   /* cache of computed terms */
static idx_t cache_size = 0;      /* number of cached elements (cache_size == last_index+1) */

/* Return the n-th term of OEIS sequence A020709 (Pisot sequence E(4,10)). */
seq_t a(idx_t n)
{
    /* Handle the first three terms directly */
    if (n == 0) return (seq_t)4;
    if (n == 1) return (seq_t)10;
    if (n == 2) return (seq_t)25;

    /* Initial allocation and precomputation */
    if (seq_cache == NULL || cache_size == 0)
    {
        cache_size = n + 1;
        seq_cache = seq_alloc(cache_size);
        seq_cache[0] = (seq_t)4;
        seq_cache[1] = (seq_t)10;
        seq_cache[2] = (seq_t)25;
        for (idx_t i = 3; i <= n; ++i)
            seq_cache[i] = 3 * seq_cache[i - 1] - 2 * seq_cache[i - 2] + 2 * seq_cache[i - 3];
        return seq_cache[n];
    }

    /* If the value is already cached, return it */
    if (n < cache_size)
        return seq_cache[n];

    /* Extend the cache to hold the new requested index */
    size_t old_size = cache_size;
    cache_size = n + 1;
    seq_cache = seq_realloc(seq_cache, old_size, cache_size);

    /* Compute only the missing terms */
    for (idx_t i = old_size; i <= n; ++i)
        seq_cache[i] = 3 * seq_cache[i - 1] - 2 * seq_cache[i - 2] + 2 * seq_cache[i - 3];

    return seq_cache[n];
}
