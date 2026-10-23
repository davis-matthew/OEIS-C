/*
 * A028167  Expansion of 1/((1-5x)(1-6x)(1-7x)(1-10x)).
 * https://oeis.org/A028167
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 28*a(n-1) - 287*a(n-2) + 1280*a(n-3) - 2100*a(n-4), n >=
 *            4 with a(0)=1, a(1)=28, a(2)=497, a(3)=7160
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 16, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

static seq_t *cache = NULL;       /* storage for memoised values */
static size_t cache_cap = 0;      /* allocated capacity (in seq_t elements) */
static uint64_t cache_len = 0;    /* number of terms actually stored */

seq_t a(idx_t n)
{
    /* Initialise cache on first call with the first four values */
    if (cache == NULL) {
        cache_cap = 4;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) {
            /* Allocation failure – return zero as a fallback */
            return 0;
        }
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)28;
        cache[2] = (seq_t)497;
        cache[3] = (seq_t)7160;
        cache_len = 4;
    }

    /* If the requested index is already cached, just return it */
    if (n < cache_len) {
        return cache[n];
    }

    /* Extend the cache until it contains a(n) */
    while (cache_len <= n) {
        /* Grow capacity if needed (doubling strategy) */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL) {
                /* Allocation failure – return zero for the missing term */
                return 0;
            }
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute next term using the 4‑term linear recurrence */
        seq_t t1 = cache[cache_len - 1];
        seq_t t2 = cache[cache_len - 2];
        seq_t t3 = cache[cache_len - 3];
        seq_t t4 = cache[cache_len - 4];

        /* Use explicit casts to seq_t for the constants */
        seq_t val = (seq_t)28    * t1
                  - (seq_t)287   * t2
                  + (seq_t)1280  * t3
                  - (seq_t)2100  * t4;

        cache[cache_len] = val;
        cache_len++;
    }

    return cache[n];
}
