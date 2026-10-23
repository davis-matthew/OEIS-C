/*
 * A049860  a(n) = Sum{a(k): k=0,1,2,...,n-4,n-2,n-1}; a(n-3) is not a
 *          summand; initial terms are 0,1,4.
 * https://oeis.org/A049860
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 36, u64 n <= 71, u128 n <= 142,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(2)=4, a(3)=5;
   a(n) = 2*a(n-1) - a(n-3) + a(n-4)   (for n ≥ 4) */
#include <stdlib.h>

static seq_t *cache      = NULL; /* a(0)..a(len-1)             */
static size_t cache_cap  = 0;    /* allocated capacity           */
static size_t cache_len  = 0;    /* number of elements computed  */

/* Ensure that cache[0..n] exists and is initialized. */
static void
ensure_cache(idx_t n)
{
    /* n is unsigned 64‑bit; cast to size_t only after checking it fits. */
    size_t need = (size_t)n + 1;

    if (cache_len > n)
        return;

    if (need > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        if (new_cap < need)
            new_cap = need;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();  /* allocation failed */
        cache      = tmp;
        cache_cap  = new_cap;
    }

    /* Initialize terms that are missing up to 'n'. */
    for (size_t i = cache_len; i < need; ++i) {
        if (i == 0)
            cache[i] = (seq_t)0;
        else if (i == 1)
            cache[i] = (seq_t)1;
        else if (i == 2)
            cache[i] = (seq_t)4;
        else if (i == 3)
            cache[i] = cache[1] + cache[2];   /* 1 + 4 = 5 */
        else
            cache[i] = (seq_t)( (seq_t)2 * cache[i-1]
                               - cache[i-3]
                               + cache[i-4] );
    }
    cache_len = need;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[(size_t)n];
}
