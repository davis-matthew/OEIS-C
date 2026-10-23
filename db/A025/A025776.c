/*
 * A025776  Expansion of 1/((1-x)*(1-x^5)*(1-x^6)).
 * https://oeis.org/A025776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A025776: Number of partitions of n into parts 1, 5 and 6
   Recurrence: a(n) = a(n-1) + a(n-5) - a(n-7) - a(n-11) + a(n-12)
   Initial values:
   a(0)=1, a(1)=1, a(2)=1, a(3)=1, a(4)=1, a(5)=2, a(6)=3,
   a(7)=3, a(8)=3, a(9)=3, a(10)=4, a(11)=5
   This implementation memoises results so that any n is
   computed in O(n) time and O(n) space. */

#include <stdlib.h>   /* malloc, realloc, free */

static const seq_t base_vals[12] = {
    1, 1, 1, 1, 1, 2, 3, 3, 3, 3, 4, 5
};

seq_t a(idx_t n)
{
    /* static cache and its current size */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* number of elements stored */

    /* If cache is empty, initialise it to size 12 with the known base values. */
    if (cache == NULL) {
        cache_size = 12;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) { /* allocation failure – return zero as a safe fallback */
            return 0;
        }
        for (idx_t i = 0; i < 12; ++i)
            cache[i] = base_vals[i];
    }

    /* Extend cache if required */
    if (n + 1 > cache_size) {
        idx_t new_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL) { /* allocation failure – return zero as a safe fallback */
            return 0;
        }
        cache = new_cache;
        /* Compute new entries */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t val;
            if (i < 12) {
                val = base_vals[i];
            } else {
                val = cache[i - 1] + cache[i - 5];
                if (i >= 7)  val -= cache[i - 7];
                if (i >= 11) val -= cache[i - 11];
                val += cache[i - 12];
            }
            cache[i] = val;
        }
        cache_size = new_size;
    }

    return cache[n];
}
