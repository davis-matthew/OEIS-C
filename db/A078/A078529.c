/*
 * A078529  Exponent sequence for a bilinear recursive sequence.
 * https://oeis.org/A078529
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + a(n-12) - 2*a(n-13) + a(n-14) with
 *            initial values a(0)=3, a(1)=1, a(2)=0, a(3)=0, a(4)=0, a(5)=0,
 *            a(6)=0, a(7)=1, a(8)=2, a(9)=3, a(10)=4, a(11)=6, a(12)=9,
 *            a(13)=10
 * Range:     u8 n <= 49, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

static const seq_t init[14] = {3,1,0,0,0,0,0,1,2,3,4,6,9,10};

static seq_t *cache = NULL; /* holds computed a(n) values */
static idx_t cache_len = 0;  /* number of elements currently stored */

seq_t a(idx_t n)
{
    /* Return the hard‑coded first 14 terms directly */
    if (n < 14)
        return init[n];

    /* Initialise the cache on the first call */
    if (cache_len == 0) {
        cache_len = 14;
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        memcpy(cache, init, cache_len * sizeof(seq_t));
    }

    /* Extend the cache if necessary */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        cache = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        for (idx_t i = cache_len; i <= n; ++i) {
            /* Compute a(i) using the linear recurrence.  All
               intermediate results are held in a signed 64‑bit
               integer to avoid unsigned underflows.             */
            int64_t val =
                (int64_t)2 * (int64_t)cache[i-1]
                - (int64_t)cache[i-2]
                + (int64_t)cache[i-12]
                - (int64_t)2 * (int64_t)cache[i-13]
                + (int64_t)cache[i-14];
            cache[i] = (seq_t)val;
        }
        cache_len = new_len;
    }

    return cache[n];
}
