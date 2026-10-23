/*
 * A140151  a(1)=1, a(n)=a(n-1)+n^2 if n odd, a(n)=a(n-1)+ n^5 if n is even.
 * https://oeis.org/A140151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 9, u32 n <= 61, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A140151: a(1) = 1;
   For n > 1, a(n) = a(n-1) + n^2 if n is odd, else a(n) = a(n-1) + n^5 if n is even. */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* Stores computed terms */
    static idx_t cache_size = 0;     /* Number of valid terms in cache */
    static size_t cache_cap = 0;     /* Capacity of the cache array */

    if (n == 0)                 /* Undefined for n < 1; safe guard */
        return 0;

    /* Allocate or grow the cache if needed */
    if (cache == NULL) {
        cache_cap = (n > 128) ? (size_t)n : 128;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
    } else if (cache_cap < (size_t)n) {
        size_t new_cap = cache_cap * 2;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(cache, sizeof(seq_t) * new_cap);
        if (!tmp) {              /* Allocation failed: clean up and return */
            free(cache);
            cache = NULL;
            cache_size = 0;
            return 0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialize first term if cache is empty */
    if (cache_size < 1) {
        cache[0] = (seq_t)1;
        cache_size = 1;
    }

    /* Compute terms up to n */
    for (idx_t i = cache_size; i < n; ++i) {
        idx_t idx = i + 1;          /* term index (1‑based) */
        seq_t add;
        if (idx & 1) {              /* odd index -> square */
            seq_t b = (seq_t)idx;
            add = b * b;
        } else {                    /* even index -> fifth power */
            seq_t b = (seq_t)idx;
            seq_t b2 = b * b;
            seq_t b3 = b2 * b;
            seq_t b4 = b3 * b;
            seq_t b5 = b4 * b;
            add = b5;
        }
        cache[i] = cache[i - 1] + add;
    }
    cache_size = n;
    return cache[n - 1];
}
