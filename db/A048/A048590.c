/*
 * A048590  Pisot sequence L(8,9).
 * https://oeis.org/A048590
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=8, a(1)=9, a(2)=11, a(3)=14, a(4)=18, a(5)=24 and for n >
 *            5: a(n) = 2*a(n-1) - a(n-2) + a(n-4) - a(n-5)
 * Range:     u8 n <= 12, u16 n <= 29, u32 n <= 64, u64 n <= 133,
 *            u128 n <= 270, big any n
 * Helpers:   ensure_cache
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/ensure_cache.h"

static seq_t *cache = NULL;
static idx_t   len   = 0;   /* number of elements currently stored */
static size_t  cap   = 0;   /* allocated capacity of the array */

/* Return the n-th term of the Pisot sequence L(8,9). */
seq_t a(idx_t n)
{
    /* Handle the initial terms directly. */
    if (n < 6) {
        switch ((int)n) {
            case 0: return (seq_t)8;
            case 1: return (seq_t)9;
            case 2: return (seq_t)11;
            case 3: return (seq_t)14;
            case 4: return (seq_t)18;
            case 5: return (seq_t)24;
        }
    }

    /* If the requested term is already cached, return it. */
    if (n < len)
        return cache[n];

    /* Ensure the cache can hold indices up to n. */
    ensure_cache(n, &cache, &cap);

    /* Initialise the first six terms if they are not yet present. */
    if (len < 6) {
        cache[0] = (seq_t)8;
        cache[1] = (seq_t)9;
        cache[2] = (seq_t)11;
        cache[3] = (seq_t)14;
        cache[4] = (seq_t)18;
        cache[5] = (seq_t)24;
        len = 6;
    }

    /* Compute missing terms up to n using the recurrence. */
    for (idx_t i = len; i <= n; ++i) {
        seq_t val = (seq_t)2 * cache[i-1]          /* 2*a(n-1) */
                  - cache[i-2]                      /* ... - a(n-2) */
                  + cache[i-4]                      /* ... + a(n-4) */
                  - cache[i-5];                     /* ... - a(n-5) */
        cache[i] = val;
    }
    len = n + 1;     /* all indices up to n are now valid */

    return cache[n];
}
