/*
 * A164392  Number of binary strings of length n with no substrings equal to
 *          0001 or 0010.
 * https://oeis.org/A164392
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, walk
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 18, u32 n <= 38, u64 n <= 78, u128 n <= 157,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-1) + a(n-2) + a(n-4) + 1  for n >= 4
   Initial values: a(0)=1, a(1)=2, a(2)=4, a(3)=8 */
#include <stdlib.h>

static seq_t *cache = NULL;     /* cached sequence values */
static idx_t cached_max = (idx_t)-1;  /* highest index cached */
static size_t cache_cap = 0;    /* allocated size of cache */

seq_t a(idx_t n)
{
    /* Handle small n directly */
    if (n < 4) {
        return (seq_t)1 << (unsigned)n;  /* 1,2,4,8 for n=0..3 */
    }

    /* Initialize cache if first call */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 1;  /* a(0) */
        cache[1] = 2;  /* a(1) */
        cache[2] = 4;  /* a(2) */
        cache[3] = 8;  /* a(3) */
        cached_max = 3;
    }

    /* If requested n already cached, return */
    if (n <= cached_max) {
        return cache[n];
    }

    /* Ensure the cache has enough space */
    if ((size_t)(n + 1) > cache_cap) {
        size_t new_cap = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) {
            /* Allocation failed; abort gracefully */
            return 0;
        }
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute values up to n */
    for (idx_t i = cached_max + 1; i <= n; ++i) {
        cache[i] = cache[i - 1] + cache[i - 2] + cache[i - 4] + 1;
    }

    cached_max = n;
    return cache[n];
}
