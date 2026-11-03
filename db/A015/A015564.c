/*
 * A015564  Expansion of x/(1 - 7*x - 6*x^2).
 * https://oeis.org/A015564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) + 6*a(n-2), with a(0)=0, a(1)=1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 866 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

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

/* Local state for memoisation */
static seq_t *seq_cache = NULL;      /* array holding computed values      */
static size_t   cache_cap  = 0;      /* currently allocated capacity      */
static idx_t    cache_len  = 0;      /* number of terms already computed */

/* Compute a(n) using recurrence a(n)=7*a(n-1)+6*a(n-2)   */
seq_t a(idx_t n)
{
    /* If we already have the requested value, return it */
    if (n < cache_len)
        return seq_cache[(size_t)n];

    /* Ensure the cache can hold at least (n+1) elements */
    ensure_cache(n, &seq_cache, &cache_cap);

    /* Initialise the first two terms if necessary */
    if (cache_len < 2) {
        seq_cache[0] = (seq_t)0;
        seq_cache[1] = (seq_t)1;
        cache_len = 2;
    }

    /* Compute missing values up to n */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_cache[(size_t)i] =
            ((seq_t)7) * seq_cache[(size_t)(i - 1)] +
            ((seq_t)6) * seq_cache[(size_t)(i - 2)];
    }

    cache_len = n + 1;
    return seq_cache[(size_t)n];
}
