/*
 * A090401  Expansion of 1/(1-3x+3x^4).
 * https://oeis.org/A090401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*a(n-1) - 3*a(n-4) Initial values: a(0)=1, a(1)=3,
 *            a(2)=9, a(3)=27
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   alloc_seq_array, seq_free_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/seq_free_array.h"

static seq_t *cache      = NULL;  /* array of computed values   */
static idx_t  cache_len  = 0;     /* number of valid elements   */
static idx_t  cache_cap  = 0;     /* current capacity of cache  */

/* Grow the cache to hold at least 'size' elements
   using the helper allocator (handles arbitrary‑precision types). */
static void ensure_cache_size(idx_t size)
{
    if (size <= cache_cap)
        return;
    seq_t *tmp = alloc_seq_array((size_t)size);
    if (tmp == NULL)
        abort();            /* allocation failed */

    /* Copy old elements to new array (default ctor already called by alloc_seq_array). */
    for (idx_t i = 0; i < cache_len; ++i)
        tmp[i] = cache[i];

    /* Free the old array (helper handles proper destructor calls). */
    seq_free_array(cache);

    cache      = tmp;
    cache_cap  = size;
}

/* Initialize cache with the first four terms if it hasn't been done yet. */
static void init_cache(void)
{
    if (cache != NULL)
        return;
    ensure_cache_size(4);
    cache[0] = (seq_t)1;
    cache[1] = (seq_t)3;
    cache[2] = (seq_t)9;
    cache[3] = (seq_t)27;
    cache_len = 4;
}

/* Return the n-th term of the sequence.  Memoises computations
   in a static cache so that calls in any order are efficient. */
seq_t a(idx_t n)
{
    init_cache();

    /* If already computed, return it. */
    if (n < cache_len)
        return cache[n];

    /* Ensure the cache can hold up to n. */
    ensure_cache_size(n + 1);

    /* Compute missing terms up to n. */
    for (idx_t i = cache_len; i <= n; ++i) {
        /* a(i) = 3*a(i-1) - 3*a(i-4) for i >= 4 */
        seq_t term1 = (seq_t)3 * cache[i - 1];
        seq_t term2 = (seq_t)3 * cache[i - 4];
        cache[i] = term1 - term2;
    }
    cache_len = n + 1;
    return cache[n];
}
