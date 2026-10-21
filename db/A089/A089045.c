/*
 * A089045  a(n) = a(n-1) + (-1)^floor(n/2)*a(floor(n/2)) with a(1) = 1.
 * https://oeis.org/A089045
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = a(n-1) + (-1)^floor(n/2)*a(floor(n/2)), a(1)=1
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   alloc_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"

static seq_t *cache       = NULL;   /* 1‑based cache (cache[0] unused) */
static idx_t  cache_cap   = 0;      /* current capacity of cache array */
static idx_t  computed    = 0;      /* highest index that has been computed */

/* Grow the cache array so that it can hold values up to 'needed' (1‑based). */
static void ensure_cache(idx_t needed)
{
    /* need at least needed+1 slots (0 is unused) */
    if (needed + 1 <= cache_cap)
        return;

    idx_t new_cap = cache_cap ? cache_cap << 1 : 32;
    while (new_cap <= needed)
        new_cap <<= 1;
    new_cap += 1;  /* one slot for index 0 */

    seq_t *new_arr = alloc_seq_array(new_cap);   /* default‑initialised to 0 */
    for (idx_t i = 0; i < cache_cap; ++i)
        new_arr[i] = cache[i];
    for (idx_t i = cache_cap; i < new_cap; ++i)
        new_arr[i] = 0;

    free(cache);
    cache     = new_arr;
    cache_cap = new_cap;
}

/* Sequence A089045: a(1) = 1, a(n) = a(n-1) + (-1)^floor(n/2)*a(floor(n/2)) for n>=2. */
seq_t a(idx_t n)
{
    if (n < 1)          /* undefined for n<1; return 0 to avoid UB */
        return 0;

    ensure_cache(n);

    /* initialise base case once */
    if (cache[1] == 0) {
        cache[1] = 1;
        computed = 1;
    }

    /* compute missing values up to n */
    if (n > computed) {
        for (idx_t i = computed + 1; i <= n; ++i) {
            seq_t term = cache[i - 1];
            seq_t add  = cache[i >> 1];         /* floor(i/2) */
            if ((i >> 1) & 1)   /* odd => negative sign */
                term -= add;
            else
                term += add;
            cache[i] = term;
        }
        computed = n;
    }

    return cache[n];
}
