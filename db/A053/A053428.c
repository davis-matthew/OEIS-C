/*
 * A053428  a(n) = a(n-1) + 20*a(n-2), n >= 2; a(0)=1, a(1)=1.
 * https://oeis.org/A053428
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 14, u64 n <= 27, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + 20*a(n-2),  a(0) = a(1) = 1 */

#include <stdlib.h>

static seq_t *cache = NULL;          /* memoized values of the sequence */
static size_t cache_len = 0;         /* number of values stored */
static size_t cache_cap = 0;         /* allocated capacity of cache */

seq_t a(idx_t n)
{
    /* ensure cache is prepared */
    if (cache_len == 0) { /* first call */
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* unlikely, but guard */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* compute higher indices on demand */
    while (cache_len <= (size_t)n) {
        /* grow cache if needed */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) {
                free(cache); /* clean up before aborting */
                cache = NULL;
                return (seq_t)0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }
        /* recurrence: a[k] = a[k-1] + 20*a[k-2] */
        seq_t val = cache[cache_len - 1] + ((seq_t)20 * cache[cache_len - 2]);
        cache[cache_len] = val;
        ++cache_len;
    }

    return cache[n];
}
