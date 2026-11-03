/*
 * A139423  a(1)=1, a(2)=2. For n >= 3, a(n) = a(n-1) + d(a(n-1)) + d(a(n-2)),
 *          where d(m) is the number of positive divisors of m.
 * https://oeis.org/A139423
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + d(a(n-1)) + d(a(n-2)), with a(1) = 1, a(2) = 2
 *            and d(m) = number of positive divisors of m
 * Range:     u8 n <= 39, u16 n <= 5093, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 80 terms through n = 9865 (2026-11-03)
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

static seq_t *cache = NULL;
static size_t capacity = 0;
static idx_t max_computed = 0;

seq_t a(idx_t n)
{
    if (n < 1)            /* below offset – undefined, but harmless */
        return (seq_t)0;

    /* Ensure space for at least two terms. */
    ensure_cache(2, &cache, &capacity);

    /* Initialise the first two terms on first use. */
    if (max_computed == 0) {
        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)2;   /* a(2) */
        max_computed = 2;
    }

    /* Resize cache if a larger index was requested. */
    if ((size_t)n > capacity)
        ensure_cache(n, &cache, &capacity);

    /* Compute all missing terms up to n. */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            /* a(i-1) is stored at index i-2, a(i-2) at i-3. */
            seq_t prev     = cache[i - 2];
            seq_t prevprev = cache[i - 3];
            uint64_t d_prev   = num_divisors((uint64_t)prev);
            uint64_t d_pprev  = num_divisors((uint64_t)prevprev);
            cache[i - 1] = prev + (seq_t)d_prev + (seq_t)d_pprev;
        }
        max_computed = n;
    }

    return cache[n - 1];
}
