/*
 * A125028  a(1)=2, a(n)=2*a(n-1)-floor(sqrt(a(n-1))).
 * https://oeis.org/A125028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1)=2, a(n)=2*a(n-1)-floor_sqrt(a(n-1)) for n>=2
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 31 (2026-11-03)
 * Helpers:   ipow_i32, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/ipow_i32.h"
#include "../utils/mem.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

static seq_t *alloc_seq_array(size_t n)
{
    return mem_new(seq_t, n);
}

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

seq_t a(idx_t n)
{
    /* static cache */
    static seq_t *cache = NULL;   /* array indexed directly by n (0 unused) */
    static size_t cap   = 0;      /* current capacity of cache (>= len+1)   */
    static size_t len   = 0;      /* highest computed index (>=1 if len>0) */

    if (n == 0)                     /* undefined for n<1, return 0 as a safety */
        return 0;

    /* Ensure cache is prepared for at least index n */
    size_t N = (size_t)n;
    if (len < N) {
        /* Grow capacity if necessary (at least N+1 for index 0) */
        size_t required = N + 1;
        if (cap < required) {
            size_t new_cap = cap ? (cap << 1) : 4;
            while (new_cap < required)
                new_cap <<= 1;

            /* Allocate new array with proper construction (delegated to utils) */
            seq_t *new_cache = alloc_seq_array(new_cap);
            if (!new_cache)
                abort();           /* allocation failure */

            /* Copy existing elements */
            if (cap > 0) {
                for (size_t i = 0; i <= len; ++i)
                    new_cache[i] = cache[i];
                free_seq_array(cache);
            }
            cache = new_cache;
            cap   = new_cap;
        }

        /* Initialize first term if not done */
        if (len == 0) {
            cache[0] = (seq_t)0;          /* unused slot */
            cache[1] = (seq_t)2;          /* a(1)=2 */
            len = 1;
        }

        /* Compute missing terms up to N */
        for (size_t i = len + 1; i <= N; ++i) {
            seq_t prev = cache[i - 1];
            seq_t s    = floor_sqrt(prev);
            cache[i]   = prev * (seq_t)2 - s;
        }
        len = N;
    }
    return cache[(size_t)n];
}
