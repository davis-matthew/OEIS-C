/*
 * A214624  Braid numbers B((2)^n->(2)^n).
 * https://oeis.org/A214624
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) * 2*n*(2*n-3)*(3*n-2) / (3*n-5), a(0) = 1, a(1) =
 *            1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* Cached values of the sequence */
static seq_t *cache = NULL;      /* array of seq_t values */
static size_t cache_size = 0;    /* allocated slots (>= size of cache) */
static size_t computed = 0;      /* next index that needs to be computed */

/* Ensure that the cache can hold indices up to n */
static void
ensure_cache(idx_t n)
{
    size_t needed = n + 1;
    if (needed <= cache_size)
        return;

    size_t new_size = cache_size ? cache_size * 2 : 1;
    while (new_size < needed)
        new_size *= 2;

    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failed – abort program */
        exit(EXIT_FAILURE);
    }
    cache = tmp;
    cache_size = new_size;
}

/* Compute a(n) using memoised recurrence */
seq_t
a(idx_t n)
{
    /* First-time initialisation of the cache */
    if (!cache) {
        cache = (seq_t *)calloc(1, sizeof(seq_t));
        if (!cache) exit(EXIT_FAILURE);
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        cache_size = 1;
        computed = 1;          /* next index to compute is 1 */
    }

    /* Resize cache if necessary */
    ensure_cache(n);

    /* Compute the missing terms up to index n */
    for (idx_t k = computed; k <= n; ++k) {
        if (k == 0) {
            cache[0] = (seq_t)1;   /* a(0) = 1 */
        } else if (k == 1) {
            cache[1] = (seq_t)1;   /* a(1) = 1 */
        } else {
            /* Use 128‑bit arithmetic when seq_t is a 64‑bit (or smaller) integer,
               else rely on wide integer operations. */
            seq_t prev = cache[k - 1];
            seq_t val;
            if (sizeof(seq_t) <= 8) {
                __int128_t t = ( __int128_t)prev;
                t *= (int64_t)(2 * (int64_t)k);
                t *= (int64_t)(2 * (int64_t)k - 3);
                t *= (int64_t)(3 * (int64_t)k - 2);
                t /= (int64_t)(3 * (int64_t)k - 5);
                val = (seq_t)t;
            } else {
                val = prev;
                val *= (seq_t)(2 * (seq_t)k);
                val *= (seq_t)(2 * (seq_t)k - 3);
                val *= (seq_t)(3 * (seq_t)k - 2);
                val /= (seq_t)(3 * (seq_t)k - 5);
            }
            cache[k] = val;
        }
    }
    computed = n + 1;    /* all indices up to n are now available */
    return cache[n];
}
