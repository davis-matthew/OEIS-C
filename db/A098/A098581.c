/*
 * A098581  Expansion of (1+2*x+4*x^2)/(1-x-8*x^4).
 * https://oeis.org/A098581
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=3, a(2)=7, a(3)=7, a(n)=a(n-1)+8*a(n-4) (n>=4)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   cast_ptr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation uses a static cache that grows only as needed.  It works for
   any build where seq_t is an arithmetic type or an arbitrary‑precision
   integer class.  The cache is resized with malloc/realloc, casting the void*
   pointer appropriately for C and for C++ builds. */

#include <stdlib.h>
#include "../utils/cast_ptr.h"

static seq_t *cache = NULL;      /* dynamic array of previously computed terms */
static idx_t cache_len = 0;      /* number of elements currently stored */

static int ensure_size(idx_t new_len)
{
    if (new_len <= cache_len)
        return 1;                /* already large enough */

    /* Allocate a larger array */
    void *tmp_raw = realloc(cache, new_len * sizeof(seq_t));
    if (!tmp_raw)
        return 0;                /* allocation failed */

    cache = cast_ptr(tmp_raw);

    /* Initialise any new entries */
    for (idx_t i = cache_len; i < new_len; ++i)
    {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)3;
        else if (i == 2)
            cache[i] = (seq_t)7;
        else if (i == 3)
            cache[i] = (seq_t)7;
        else
            cache[i] = cache[i - 1] + ((seq_t)8) * cache[i - 4];
    }
    cache_len = new_len;
    return 1;
}

seq_t a(idx_t n)
{
    if (!ensure_size(n + 1))
        return (seq_t)0;          /* fallback if allocation fails */

    return cache[n];
}
