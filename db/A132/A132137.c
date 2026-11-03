/*
 * A132137  a(1)=1, a(n)=maximal digit of sum a(1)+...+a(n-1).
 * https://oeis.org/A132137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(1) = 1; for n > 1, a(n) = maximal decimal digit of
 *            sum_{k=1}^{n-1} a(k)
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Validated: partial, pass, 105 terms through n = 105 (2026-11-03)
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

static seq_t *cache = NULL;      /* 1‑indexed cache of a(i)   */
static size_t cap    = 0;        /* allocated size of cache   */
static idx_t   known  = 0;       /* largest index already computed */
static unsigned long long sum_val = 0;  /* running sum of a(1)…a(known) */

seq_t a(idx_t n)
{
    if (n == 0)          /* offset is 1; n==0 is out­of‑range */
        return (seq_t)0;

    /* allocate or grow the cache to hold indices up to n */
    if (!cache || cap < (size_t)(n + 1))
        ensure_cache((idx_t)(n + 1), &cache, &cap);

    /* fast return if we already know this term */
    if (n <= known)
        return cache[n];

    /* initialise the very first term */
    if (known == 0) {
        cache[1]  = (seq_t)1;
        sum_val   = 1ull;
        known     = 1;
    }

    /* compute all missing terms up to n */
    for (idx_t i = known + 1; i <= n; ++i) {
        /* maximum decimal digit of sum_val */
        unsigned long long tmp = sum_val;
        unsigned int dmax = 0;
        while (tmp > 0) {
            unsigned int d = (unsigned int)(tmp % 10ull);
            if (d > dmax) dmax = d;
            tmp /= 10ull;
        }
        seq_t val = (seq_t)dmax;
        cache[i] = val;
        sum_val += (unsigned long long)val;
    }
    known = n;
    return cache[n];
}
