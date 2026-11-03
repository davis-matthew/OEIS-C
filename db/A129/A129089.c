/*
 * A129089  a(0)=a(1)=1. a(n) = gcd(a(n-1), n) + gcd(a(n-2), n).
 * https://oeis.org/A129089
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = gcd(a(n-1), n) + gcd(a(n-2), n); a(0) = a(1) = 1
 * Range:     u8 n <= 621, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 120 terms through n = 959 (2026-11-03)
 * Helpers:   gcd, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd.h"
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
static size_t cap = 0;
static idx_t max_comp = 1;  /* indices 0 and 1 are known */

/* Return a(n) for all n >= 0, memoising results.  The function can be
   called in any order; previously computed values are reused. */
seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;               /* a(0) = a(1) = 1 */

    /* Make sure the cache can hold terms up to n. */
    ensure_cache(n, &cache, &cap);

    /* Initialise the first two terms if they are not already set. */
    if (cache[0] == 0)
        cache[0] = 1;
    if (cache[1] == 0)
        cache[1] = 1;

    /* Compute any terms that have not yet been evaluated. */
    if (n > max_comp)
    {
        /* max_comp is at least 1 here (first two terms are ready). */
        for (idx_t i = max_comp + 1; i <= n; ++i)
        {
            seq_t g1 = gcd(cache[i-1], (seq_t)i);
            seq_t g2 = gcd(cache[i-2], (seq_t)i);
            cache[i] = g1 + g2;
        }
        max_comp = n;
    }

    return cache[n];
}
