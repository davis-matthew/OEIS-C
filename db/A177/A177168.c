/*
 * A177168  Sequence defined by the recurrence formula
 *          a(n+1)=sum(a(p)*a(n-p)+k,p=0..n)+l for n>=1, with here a(0)=1,
 *          a(1)=6, k=0 and l=-2.
 * https://oeis.org/A177168
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = sum_{p=0}^{n-1} a(p)*a(n-1-p) - 2 (with a(0)=1, a(1)=6).
 *            The sequence is A177168
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include "../utils/alloc_seq_array.h"

/* static cache for memoization */
static seq_t *seq_cache = NULL;   /* array of computed a(n) values   */
static size_t cache_len = 0;      /* number of values currently stored */
static size_t cache_cap = 0;      /* capacity of seq_cache            */

/* Ensure that seq_cache has capacity >= needed.
 * If needed > cache_cap, allocate a larger array, copy existing values,
 * free old memory, and assign the new pointer. */
static void ensure_cache_capacity(size_t needed)
{
    if (cache_cap >= needed)
        return;
    /* Allocate new array */
    seq_t *new_cache = alloc_seq_array(needed);
    /* Copy existing values if any */
    for (size_t i = 0; i < cache_len; ++i)
        new_cache[i] = seq_cache[i];
    /* Free old cache */
    free(seq_cache);
    seq_cache = new_cache;
    cache_cap = needed;
}

/* Compute a(n) with memoization.  The function may be called with
 * arbitrary orders of n.  For values that are already cached, the
 * result is returned directly. */
seq_t a(idx_t n)
{
    /* Handle the first two terms directly */
    if (n == 0)
        return 1;
    if (n == 1)
        return 6;

    /* Ensure the cache is large enough for requested n */
    size_t need = (size_t)n + 1;   /* indices 0..n inclusive */
    ensure_cache_capacity(need);

    /* Initialize cache entries for a(0) and a(1) if not already done */
    if (cache_len == 0) {
        seq_cache[0] = 1;
        /* Guard against n==0 case above; but we need a[1] as well */
        if (need > 1)
            seq_cache[1] = 6;
        cache_len = 2;
    }
    else if (cache_len == 1) {
        seq_cache[0] = 1;
        if (need > 1)
            seq_cache[1] = 6;
        cache_len = 2;
    }

    /* Compute missing terms up to n */
    for (size_t k = cache_len; k <= (size_t)n; ++k) {
        /* Compute sum_{p=0}^{k-1} a(p)*a(k-1-p) */
        seq_t sum = 0;
        for (idx_t p = 0; p < (idx_t)k; ++p) {
            sum += seq_cache[p] * seq_cache[(idx_t)k - 1 - p];
        }
        /* Apply the constant l = -2 */
        sum -= 2;   /* subtracting 2 works in unsigned arithmetic          */
        seq_cache[k] = sum;
    }
    cache_len = (size_t)n + 1;   /* now all terms up to n are stored */

    return seq_cache[n];
}
