/*
 * A135491  Number of ways to toss a coin n times and not get a run of four.
 * https://oeis.org/A135491
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2) + a(n-3) (n > 3) a(0) = 1, a(1) = 2, a(2)
 *            = 4, a(3) = 8 This recurrence is equivalent to a(n) =
 *            2*A000073(n+2)
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 36, u64 n <= 72, u128 n <= 145,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

/* static cache of previously computed values */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of elements stored (max index + 1) */

seq_t a(idx_t n)
{
    /* enlarge cache if requested index not yet present */
    if ((size_t)n >= cache_len) {
        size_t new_len = (size_t)n + 1;
        seq_t *new_cache = alloc_seq(new_len);

        /* copy old values that are already known */
        for (size_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];

        /* fill the remaining slots up to new_len-1 */
        for (size_t i = cache_len; i < new_len; ++i) {
            if (i == 0)          new_cache[0] = (seq_t)1;
            else if (i == 1)     new_cache[1] = (seq_t)2;
            else if (i == 2)     new_cache[2] = (seq_t)4;
            else if (i == 3)     new_cache[3] = (seq_t)8;
            else                 new_cache[i] = new_cache[i-1] + new_cache[i-2] + new_cache[i-3];
        }

        /* free old storage, replace with new expanded cache */
        if (cache) free_seq(cache);
        cache = new_cache;
        cache_len = new_len;
    }

    return cache[(size_t)n];
}
