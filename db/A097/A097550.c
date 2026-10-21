/*
 * A097550  Number of positive words of length n in the monoid Br_3 of
 *          positive braids on 4 strands.
 * https://oeis.org/A097550
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 2*a(n-2) + a(n-3) with a(0) = 1, a(1) = 3,
 *            a(2) = 8
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 25, u64 n <= 52, u128 n <= 104,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* extend the cache to cover index n */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) {
            /* allocation failure – abort as the build expects a non‑zero value */
            abort();
        }
        cache = tmp;

        /* initialise the new entries */
        for (idx_t i = cache_size; i < new_size; ++i) {
            if (i == 0) {
                cache[i] = (seq_t)1;
            } else if (i == 1) {
                cache[i] = (seq_t)3;
            } else if (i == 2) {
                cache[i] = (seq_t)8;
            } else {
                /* recurrence: a(n) = 3*a(n-1) - 2*a(n-2) + a(n-3) */
                seq_t term1 = cache[i - 1] * (seq_t)3;
                seq_t term2 = cache[i - 2] * (seq_t)2;
                cache[i] = term1 - term2 + cache[i - 3];
            }
        }
        cache_size = new_size;
    }
    return cache[n];
}
