/*
 * A231960  Powers of 3 together with multiples of 6.
 * https://oeis.org/A231960
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 46, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: union of powers of 3 and multiples of 6 (offset 1) */
/* The sequence is the sorted merge of the two disjoint sets:
   {3^k | k ≥ 0} and {6·k | k ≥ 1}.  No arithmetic formula is known,
   so we generate it lazily and cache the results. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for the sequence */
    static seq_t *cache  = NULL;
    static idx_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;  /* allocated capacity */

    /* static variables holding the next values to merge */
    static seq_t next_power = 1;   /* 3^k, starting with k = 0 */
    static seq_t next_mult  = 6;   /* 6·k, starting with k = 1 */

    /* Grow the cache until it contains at least n terms. */
    if (n > cache_len) {
        /* Allocate or expand the array. */
        if (cache == NULL) {
            cache_cap = 16;
            cache = (seq_t *)realloc(NULL, cache_cap * sizeof(seq_t));
        } else if (cache_cap < (size_t)n) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            while (new_cap < (size_t)n) new_cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp) {
                cache = tmp;
                cache_cap = new_cap;
            }
        }

        /* Generate missing terms by merging the two sorted streams. */
        while (cache_len < n) {
            if (next_power < next_mult) {
                cache[cache_len++] = next_power;
                next_power *= 3;
            } else {
                cache[cache_len++] = next_mult;
                next_mult += 6;
            }
        }
    }

    /* Return the requested term (offset 1). */
    return cache[n - 1];
}
