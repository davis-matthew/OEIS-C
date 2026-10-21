/*
 * A068342  a(1) = 1; a(n+1) = (product{k|n} a(k)) (sum{j|n} 1/a(j)), where
 *          both the product and sum are over the positive divisors of n.
 * https://oeis.org/A068342
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; a(n+1) = (∏_{k|n} a(k)) * (∑_{j|n} 1/a(j)) for n ≥ 1
 *            (i.e. a(m) depends on the divisors of m‑1)
 * Range:     u8 n <= 8, u16 n <= 12, u32 n <= 16, u64 n <= 20, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>

/* static state for memoisation */
static seq_t *cache       = NULL;   /* cache[i] holds a(i), 1‑based          */
static size_t cache_cap   = 0;      /* allocated slots including index 0     */
static idx_t  computed_max = 0;     /* largest n that has been computed      */

/* make sure the cache can address indices 0 … n inclusive */
static void ensure_cap(idx_t n)
{
    size_t needed = (size_t)n + 1;
    if (needed <= cache_cap)
        return;
    seq_t *new_cache = (seq_t*)malloc(needed * sizeof(seq_t));
    if (!new_cache)
        abort();                     /* allocation failure */

    if (cache) {
        /* copy already computed values (0 … computed_max) */
        memcpy(new_cache, cache, (computed_max + 1) * sizeof(seq_t));
        free(cache);
    }

    cache     = new_cache;
    cache_cap = needed;
}

seq_t a(idx_t n)
{
    if (n == 0)                  /* undefined for n=0 – harmless fallback */
        return (seq_t)0;

    /* initialise a(1) on first call */
    if (computed_max == 0) {
        ensure_cap(1);
        cache[0] = (seq_t)0;     /* unused, but keep defined */
        cache[1] = (seq_t)1;
        computed_max = 1;
    }

    if (n <= computed_max)
        return cache[n];

    ensure_cap(n);

    /* compute missing terms up to n */
    for (idx_t m = computed_max + 1; m <= n; ++m) {
        idx_t nd = m - 1;        /* divisors of nd determine a(m) */

        /* gather divisors of nd */
        idx_t divs[128];         /* 10000 has < 128 divisors for sure */
        size_t dcnt = 0;
        for (idx_t d = 1; d * d <= nd; ++d) {
            if (nd % d == 0) {
                divs[dcnt++] = d;
                if (d != nd / d)
                    divs[dcnt++] = nd / d;
            }
        }
        /* product of a(k) over all divisors k of nd */
        seq_t prod = (seq_t)1;
        for (size_t i = 0; i < dcnt; ++i)
            prod *= cache[divs[i]];

        /* sum of prod / a(d) for each divisor d of nd */
        seq_t sum = (seq_t)0;
        for (size_t i = 0; i < dcnt; ++i) {
            seq_t term = prod / cache[divs[i]];   /* exact division */
            sum += term;
        }

        cache[m] = sum;
    }

    computed_max = n;
    return cache[n];
}
