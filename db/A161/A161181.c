/*
 * A161181  A117805/3.
 * https://oeis.org/A161181
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(n)=a(n-1)*(3*a(n-1)-1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A161181 (recurrence).  */
#include <stddef.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t  cache_size = 0;
static size_t cache_cap  = 0;

static void grow_cache(idx_t needed)
{
    if (needed < cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 8;
    while (new_cap <= (size_t)needed) new_cap *= 2;
    cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache_cap = new_cap;
}

seq_t a(idx_t n)
{
    /* Ensure cache is large enough */
    if (cache == NULL) {
        grow_cache(0);
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    if (n >= cache_size) {
        grow_cache(n);
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            /* tmp = prev * (3*prev - 1) */
            seq_t tmp = prev * ((seq_t)3 * prev - (seq_t)1);
            cache[i] = tmp;
        }
        cache_size = n + 1;
    }
    return cache[n];
}
