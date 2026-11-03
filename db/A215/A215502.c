/*
 * A215502  a(n) = (1+sqrt(3))^n + (-2)^n + (1-sqrt(3))^n + 1.
 * https://oeis.org/A215502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 6*a(n-2) - 2*a(n-3) - 4*a(n-4) with a(0)=4,
 *            a(1)=1, a(2)=13, a(3)=13 (derived from a(n) = (1+√3)^n + (-2)^n
 *            + (1-√3)^n + 1)
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 44, u128 n <= 88,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 975 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    /* Static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;  /* number of valid elements (next free index) */
    static size_t cache_cap = 0;  /* allocated capacity */

    /* Initial seed values */
    static const seq_t init[4] = { (seq_t)4, (seq_t)1, (seq_t)13, (seq_t)13 };

    /* First time: allocate and initialise cache with the seed values */
    if (cache == NULL) {
        cache_cap = 4;
        cache_len = 4;
        cache = mem_new(seq_t, cache_cap);
        if (cache == NULL) /* allocation failed */
            return (seq_t)0;
        for (size_t i = 0; i < 4; ++i)
            cache[i] = init[i];
    }

    /* Resize cache if necessary */
    if ((size_t)n >= cache_len) {
        size_t need = (size_t)n + 1;
        if (need > cache_cap) {
            size_t new_cap = cache_cap ? cache_cap << 1 : 16;
            if (new_cap < need) new_cap = need;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (tmp == NULL) /* reallocation failed */
                return (seq_t)0;
            cache = tmp;
            cache_cap = new_cap;
        }

        /* Compute missing terms using the recurrence */
        for (size_t i = cache_len; i <= (size_t)n; ++i) {
            seq_t t1 = cache[i - 1];
            seq_t t2 = cache[i - 2];
            t2 *= (seq_t)6;
            seq_t t3 = cache[i - 3];
            t3 *= (seq_t)2;
            seq_t t4 = cache[i - 4];
            t4 *= (seq_t)4;
            cache[i] = t1 + t2 - t3 - t4;
        }
        cache_len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
