/*
 * A165323  a(0)=1, a(1)=8, a(n)=17*a(n-1)-64*a(n-2) for n>1.
 * https://oeis.org/A165323
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1; a(1)=8; a(n)=17*a(n-1)-64*a(n-2) for n>1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   alloc_seq_array, seq_free, seq_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#include "../utils/seq_one.h"
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_free.h"

static seq_t *cache = NULL;          /* cache of computed values */
static size_t cache_size = 0;        /* allocated size of cache   */
static idx_t computed = 0;           /* highest index computed   */

seq_t a(idx_t n)
{
    /* Ensure the cache is allocated and large enough. */
    if (cache == NULL) {
        size_t sz = (n < 1) ? 2U : (size_t)n + 1U;   /* at least two slots */
        cache = alloc_seq_array(sz);
        cache_size = sz;
        cache[0]  = seq_one();        /* a(0) = 1 */
        cache[1]  = (seq_t)8;        /* a(1) = 8 */
        computed = 1;                /* highest computed index */
    } else if ((size_t)n + 1U > cache_size) {
        size_t new_sz = (size_t)n + 1U;
        seq_t *tmp = alloc_seq_array(new_sz);
        for (size_t i = 0; i < cache_size; ++i)
            tmp[i] = cache[i];
        seq_free(cache);
        cache = tmp;
        cache_size = new_sz;
    }

    /* Compute missing terms up to index n. */
    if (computed < n) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t term = (seq_t)17 * cache[(size_t)(i - 1)]
                       - (seq_t)64 * cache[(size_t)(i - 2)];
            cache[(size_t)i] = term;
        }
        computed = n;
    }

    return cache[(size_t)n];
}
