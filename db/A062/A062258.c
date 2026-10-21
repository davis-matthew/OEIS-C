/*
 * A062258  Number of (0,1)-strings of length n not containing the substring
 *          0100100.
 * https://oeis.org/A062258
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A062258: Number of (0,1)-strings of length n not containing the substring 0100100
   Recurrence (valid for n ≥ 8):
       a(n) = 2*a(n-1) - a(n-3) + 2*a(n-4) - a(n-6) + a(n-7)
   Initial values (n = 0 … 7):
       a(0)=1, a(1)=2, a(2)=4, a(3)=8, a(4)=16, a(5)=32, a(6)=64, a(7)=127
*/

#include <stdlib.h>

static const seq_t init_vals[8] = {1, 2, 4, 8, 16, 32, 64, 127};

static seq_t *cache = NULL;
static size_t cache_cap = 0;
static int cache_init = 0;

static void grow_cache(size_t upto)
{
    if (!cache_init)
    {
        /* allocate at least for indices 0 … 7 */
        size_t cap = 8;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        for (size_t i = 0; i < cap; ++i) cache[i] = init_vals[i];
        cache_cap = cap;
        cache_init = 1;
    }

    if (upto + 1 <= cache_cap) return;

    size_t new_cap = cache_cap * 2;
    if (new_cap < upto + 1) new_cap = upto + 1;
    cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
    /* compute newly allocated entries */
    for (size_t i = cache_cap; i < new_cap; ++i)
    {
        if (i < 8)
        {
            cache[i] = init_vals[i];
        }
        else
        {
            /* compute a(i) using the recurrence */
            seq_t val = cache[i - 1];
            val = val * 2;               /* 2*a(i-1) */
            val = val + (cache[i - 4] * 2);   /* + 2*a(i-4) */
            val = val + cache[i - 7];   /* + a(i-7) */
            val = val - cache[i - 3];   /* - a(i-3) */
            val = val - cache[i - 6];   /* - a(i-6) */
            cache[i] = val;
        }
    }
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Ensure cache computed up to index n */
    if (n >= cache_cap) grow_cache((size_t)n);
    return cache[(size_t)n];
}
