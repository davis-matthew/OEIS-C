/*
 * A207108  Number of n X 5 0..1 arrays avoiding 0 0 0 and 0 1 1 horizontally
 *          and 0 0 1 and 1 1 0 vertically.
 * https://oeis.org/A207108
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 125, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A207108: Number of n×5 0..1 arrays avoiding 0 0 0 / 0 1 1 horizontally
   and 0 0 1 / 1 1 0 vertically.
   Recurrence (n > 6):
       a(n) = 6*a(n-1) - 15*a(n-2) + 20*a(n-3)
              - 15*a(n-4) + 6*a(n-5) - a(n-6)
   Initial values:
       a(1)=13, a(2)=169, a(3)=677, a(4)=1835,
       a(5)=4047, a(6)=7837 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache the sequence values.  cache[1..cached_len] holds a(1)..a(cached_len).  */
    static seq_t *cache = NULL;
    static idx_t cached_len = 0;

    /* Fixed initial values (index 0 unused). */
    static const seq_t init[7] = {
        (seq_t)0,    /* a(0) not defined, placeholder */
        (seq_t)13,   /* a(1) */
        (seq_t)169,  /* a(2) */
        (seq_t)677,  /* a(3) */
        (seq_t)1835, /* a(4) */
        (seq_t)4047, /* a(5) */
        (seq_t)7837  /* a(6) */
    };

    if (n == 0)            /* offset starts at 1; undefined for n==0 */
        return (seq_t)0;

    /* Ensure the cache can store up to index n. */
    if (n > cached_len) {
        size_t new_cap = (size_t)n + 1;           /* +1 because we index from 1 */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            /* Reallocation failed – fallback to abort. */
            abort();
        }
        cache = tmp;

        /* If the cache is newly created or not yet fully initialised,
           copy the known initial values into the appropriate slots. */
        if (cached_len < 6) {
            idx_t start = cached_len + 1;
            for (idx_t i = start; i <= n && i <= 6; ++i) {
                cache[(size_t)i] = init[(size_t)i];
            }
        }

        /* Compute missing terms using the recurrence. */
        for (idx_t i = cached_len + 1; i <= n; ++i) {
            if (i <= 6) {          /* already handled above */
                continue;
            }
            /* a(i) = 6*a(i-1) - 15*a(i-2) + 20*a(i-3)
                      - 15*a(i-4) + 6*a(i-5) - a(i-6) */
            seq_t t1 = 6 * cache[(size_t)(i - 1)];
            seq_t t2 = 15 * cache[(size_t)(i - 2)];
            seq_t t3 = 20 * cache[(size_t)(i - 3)];
            seq_t t4 = 15 * cache[(size_t)(i - 4)];
            seq_t t5 = 6 * cache[(size_t)(i - 5)];
            seq_t t6 = cache[(size_t)(i - 6)];
            cache[(size_t)i] = t1 - t2 + t3 - t4 + t5 - t6;
        }
        cached_len = n;
    }

    return cache[(size_t)n];
}
