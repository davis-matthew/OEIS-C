/*
 * A079898  a(1) = 1; a(n) = tau(n) - tau(n-1)* a(n-1) if n > 1.
 * https://oeis.org/A079898
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(1) = 1; a(n) = tau(n) - tau(n-1)*a(n-1) for n>1; tau is the
 *            divisor count σ₀
 * Range:     i8 n <= 7, i16 n <= 12, i32 n <= 21, i64 n <= 36, i128 n <= 66,
 *            big any n
 * Validated: standard, pass, 45 terms through n = 735 (2026-11-03)
 * Helpers:   mem, num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/num_divisors.h"
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

/* Static cache for already computed terms. */
static seq_t *cache     = NULL;
static size_t cache_cap = 0;
static idx_t cache_upto = 0;

seq_t a(idx_t n)
{
    if (n == 0) return 0;                      /* offset is 1 – 0 is undefined. */

    /* Allocate or grow the cache so that indices 1..n are accessible. */
    ensure_cache(n, &cache, &cache_cap);

    /* Ensure the initial value a(1) is set. */
    if (cache_upto < 1)
    {
        cache[1] = 1;
        cache_upto = 1;
    }

    /* Compute any missing values up to n. */
    for (idx_t i = cache_upto + 1; i <= n; ++i)
    {
        if (i == 1)
            cache[i] = 1;
        else
        {
            seq_t tau_prev = num_divisors(i - 1);
            seq_t tau_curr = num_divisors(i);
            cache[i] = tau_curr - tau_prev * cache[i - 1];
        }
    }

    cache_upto = n;
    return cache[n];
}
