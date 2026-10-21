/*
 * A206452  Number of 0..5 arrays of length n avoiding the consecutive pattern
 *          0..5.
 * https://oeis.org/A206452
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*a(n-1) - a(n-6) Initial values: a(0)=1, a(1)=6,
 *            a(2)=36, a(3)=216, a(4)=1296, a(5)=7776 Recurrence holds for n
 *            >= 6. Computation is memoized
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;
static idx_t cache_cap   = 0;

seq_t a(idx_t n)
{
    /* initialise cache once */
    if (!cache) {
        /* at least 6 entries for first terms */
        cache_cap = 6;
        cache = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();  /* allocation failure */

        const seq_t init[6] = { (seq_t)1, (seq_t)6, (seq_t)36, (seq_t)216,
                                (seq_t)1296, (seq_t)7776 };
        for (idx_t i = 0; i < 6; ++i)
            cache[i] = init[i];
        cache_size = 6;
    }

    /* extend cache to cover n */
    while (cache_size <= n) {
        /* grow capacity if necessary */
        if (cache_size == cache_cap) {
            idx_t new_cap = cache_cap * 2;
            if (new_cap <= cache_size) new_cap = cache_size + 1;
            seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute next term */
        seq_t val;
        if (cache_size < 6) {
            /* should not happen because we already seeded 6 terms */
            val = (seq_t)1;  /* placeholder */
        } else {
            val = 6 * cache[cache_size - 1] - cache[cache_size - 6];
        }
        cache[cache_size] = val;
        ++cache_size;
    }

    return cache[n];
}
