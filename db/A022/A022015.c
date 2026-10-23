/*
 * A022015  a(n)=2a(n-1)+3a(n-2)+2a(n-3)+3a(n-4).
 * https://oeis.org/A022015
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n)=2a(n-1)+3a(n-2)+2a(n-3)+3a(n-4), a(0)=2, a(1)=6, a(2)=19,
 *            a(3)=61
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 37, u128 n <= 75,
 *            big any n
 * Helpers:   alloc_seq_array, free_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_cache.h"

static seq_t *cache  = NULL;  /* cache array */
static size_t allocated = 0;  /* allocated size of cache array */
static idx_t  computed  = 0;  /* number of values already computed */

/* Compute a(n) for the OEIS sequence A022015 */
seq_t a(idx_t n)
{
    /* Initialize base terms on first use */
    if (computed == 0) {
        if (allocated < 4) {
            seq_t *tmp = alloc_seq_array(4);
            free_cache(cache);
            cache = tmp;
            allocated = 4;
        }
        cache[0] = 2;
        cache[1] = 6;
        cache[2] = 19;
        cache[3] = 61;
        computed = 4;
    }

    /* Return already known value */
    if (n < computed)
        return cache[n];

    /* Resize cache array if necessary */
    if (allocated <= n) {
        size_t new_alloc = (size_t)(n + 1);
        seq_t *tmp = alloc_seq_array(new_alloc);
        for (size_t i = 0; i < computed; ++i)
            tmp[i] = cache[i];
        free_cache(cache);
        cache = tmp;
        allocated = new_alloc;
    }

    /* Compute missing terms up to n */
    for (idx_t i = computed; i <= n; ++i) {
        cache[i] = 2 * cache[i - 1] + 3 * cache[i - 2]
                 + 2 * cache[i - 3] + 3 * cache[i - 4];
    }
    computed = n + 1;

    return cache[n];
}
