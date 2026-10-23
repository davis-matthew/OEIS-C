/*
 * A160695  Integers m such that 3*m+1 and 7*m+1 are both perfect squares.
 * https://oeis.org/A160695
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A160695: Integers m such that 3*m+1 and 7*m+1 are perfect squares.
 * Recurrence: a(1)=0, a(2)=5, a(n)=23*a(n-1) - a(n-2) + 5  (n≥3)
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_len = 0;

    if (n == 0) return 0;  /* safety, offset is 1 */

    /* Grow cache if necessary */
    if ((size_t)n > cache_len) {
        /* Allocate at least n elements; double to reduce realloc frequency */
        size_t new_cap = cache_cap;
        if (new_cap < n) new_cap = n;
        while (new_cap < n) new_cap <<= 1;

        seq_t *new_cache;
        if (cache) {
            new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        } else {
            new_cache = (seq_t *)malloc(new_cap * sizeof(seq_t));
        }

        if (!new_cache) {
            /* Allocation failure – return 0 or abort */
            return 0;
        }

        cache = new_cache;
        cache_cap = new_cap;

        /* Compute missing terms */
        for (size_t i = cache_len; i < (size_t)n; ++i) {
            seq_t val;
            if (i == 0)          /* a(1) */
                val = 0;
            else if (i == 1)     /* a(2) */
                val = 5;
            else                 /* recurrence */
                val = 23 * cache[i - 1] - cache[i - 2] + 5;
            cache[i] = val;
        }
        cache_len = (size_t)n;
    }

    return cache[(size_t)n - 1];
}
