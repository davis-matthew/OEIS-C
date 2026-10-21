/*
 * A096273  a(0)=0, then a(n)=a(n-1)+(n-1) if n is odd, a(n)=a(n/2)+n/2
 *          otherwise.
 * https://oeis.org/A096273
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0; for n>0: if n is odd, a(n)=a(n-1)+(n-1); if n is even,
 *            a(n)=a(n/2)+n/2
 * Range:     u8 n <= 94, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;
static size_t cache_alloc = 0;     /* total allocated elements */
static size_t cache_computed = 0;  /* first index not yet computed */

/* expand the cache to hold at least `needed` elements */
static void ensure_alloc(size_t needed)
{
    if (needed <= cache_alloc) return;
    size_t new_alloc = (cache_alloc > 0) ? cache_alloc : 1;
    while (new_alloc <= needed) new_alloc <<= 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
    if (!tmp) abort();   /* allocation failed */
    cache = tmp;
    cache_alloc = new_alloc;
}

/* external sequence function */
seq_t a(idx_t n)
{
    size_t idx = (size_t)n;
    if (idx >= cache_computed)
    {
        ensure_alloc(idx + 1);
        for (size_t k = cache_computed; k <= idx; ++k)
        {
            if (k == 0)
                cache[0] = 0;
            else if (k & 1) /* k is odd */
                cache[k] = cache[k - 1] + (seq_t)(k - 1);
            else            /* k is even */
                cache[k] = cache[k / 2] + (seq_t)(k / 2);
        }
        cache_computed = idx + 1;
    }
    return cache[idx];
}
