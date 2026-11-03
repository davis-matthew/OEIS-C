/*
 * A084978  Number of ways to represent n as a+b*(c+d*(e+f*(...x+y*(z)...)))
 *          in positive integers.
 * https://oeis.org/A084978
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Validated: standard, pass, 48 terms through n = 3261 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 1; a(n+1) = a(n) +  ∑_{d|n} a(d)  for n ≥ 1 */
#include <stddef.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Make sure the cache is large enough */
static void ensure_cache(idx_t n, seq_t **cache_ptr, size_t *cap_ptr) {
    seq_t *cache = *cache_ptr;
    size_t cap = *cap_ptr;
    if (cap <= (size_t)n) {
        size_t new_cap = cap ? (cap << 1) : 16;
        while (new_cap <= (size_t)n)
            new_cap <<= 1;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp && new_cap > 0) {
            /* Allocation failed – abort the program. */
            exit(1);
        }
        if (tmp) {
            /* Initialize newly allocated slots to zero. */
            if (!cache) {
                for (size_t i = 0; i < new_cap; ++i)
                    tmp[i] = 0;
            } else {
                for (size_t i = cap; i < new_cap; ++i)
                    tmp[i] = 0;
            }
            *cache_ptr = tmp;
            *cap_ptr = new_cap;
        }
    }
}

static seq_t *cache = NULL;          /* array holding computed values */
static size_t cache_cap = 0;         /* allocated size of the array   */
static idx_t  last_computed = 0;     /* highest index that has been computed */

seq_t a(idx_t n)
{
    /*  n is a positive integer (offset 1), behaviour for n<1 is undefined */
    if (n < 1) return 0;

    /* Ensure the array is large enough to address indices 1 … n. */
    /*  ensure_cache guarantees that cache has at least n+1 elements.  */
    ensure_cache(n, &cache, &cache_cap);

    /* Initialise the sequence if we have not yet done so.  */
    if (last_computed == 0) {
        cache[1] = (seq_t)1;          /* a(1) = 1  */
        last_computed = 1;
    }

    /* Compute any missing terms up to a(n).  */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t sum = 0;
        idx_t m = i - 1;                    /* the divisor number we are summing over */
        for (idx_t d = 1; d * d <= m; ++d) {
            if (m % d == 0) {
                sum += cache[d];
                idx_t e = m / d;
                if (e != d) sum += cache[e];
            }
        }
        cache[i] = cache[i - 1] + sum;      /* a(i) = a(i-1)+∑_{d|i-1}a(d) */
    }
    last_computed = n;

    return cache[n];
}
