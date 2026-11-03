/*
 * A231507  a(n) is smallest number greater than a(n-1) such that a(n)+a(n-1)
 *          is composite.
 * https://oeis.org/A231507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 112, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 82 terms through n = 994 (2026-11-03)
 * Helpers:   is_prime_seq, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: a(n) is the smallest integer > a(n-1) such that a(n)+a(n-1) is composite.
   Initial value: a(1) = 4.  All other values are generated on demand. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include "../utils/is_prime_seq.h"
#include "../utils/mem.h"

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

seq_t a(idx_t n)
{
    /* cache of computed terms – 1‑based index */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored (>=0) */
    static idx_t cache_cap = 0;   /* allocated capacity */

    if (n == 0)          /* offset is 1 – return 0 for undefined indices */
        return (seq_t)0;

    /* lazy initialization of the cache */
    if (cache == NULL) {
        cache_cap = 64;                                /* initial capacity */
        cache = seq_alloc_new_array(cache_cap + 1);    /* indices 0..cap */
        if (cache == NULL)                            /* allocation failed */
            return (seq_t)0;
        cache[1] = (seq_t)4;                           /* a(1)=4 */
        cache_len = 1;
    }

    /* grow the cache if needed */
    if (n > cache_len) {
        if (n > cache_cap) {                          /* extend capacity */
            idx_t new_cap = cache_cap;
            while (new_cap < n) new_cap <<= 1;
            seq_t *tmp = seq_alloc_new_array(new_cap + 1);
            if (tmp == NULL)                         /* allocation failed */
                return (seq_t)0;
            /* copy existing values */
            for (idx_t i = 1; i <= cache_len; ++i)
                tmp[i] = cache[i];
            cache = tmp;
            cache_cap = new_cap;
        }

        /* compute missing terms */
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            seq_t candidate = prev + (seq_t)1;
            /* find first candidate such that candidate + prev is composite */
            while (is_prime_seq(candidate + prev))
                ++candidate;
            cache[i] = candidate;
        }
        cache_len = n;
    }

    return cache[n];
}
