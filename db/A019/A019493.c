/*
 * A019493  a(n) = 3*a(n-1) - 4*a(n-3) + a(n-6).
 * https://oeis.org/A019493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - 4*a(n-3) + a(n-6) Initial values: a(0)=4,
 *            a(1)=9, a(2)=20, a(3)=44, a(4)=96, a(5)=209 Recurrence valid for
 *            n >= 6
 * Range:     u8 n <= 5, u16 n <= 12, u32 n <= 26, u64 n <= 55, u128 n <= 112,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;      /* dynamically sized array of sequence values */
static idx_t  cache_cap = 0;     /* allocated capacity of cache */
static idx_t  max_computed = 0;  /* highest index whose value is guaranteed in cache */

/* Ensure 'cache' has at least (need+1) elements and compute all missing values up to 'need'. */
static void ensure_cache(idx_t need)
{
    /* Grow array capacity if necessary */
    if (cache_cap <= need) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 16;
        while (new_cap <= (size_t)need) new_cap <<= 1;
        seq_t *new_arr = (seq_t *)malloc(new_cap * sizeof(seq_t));
        if (!new_arr) abort();                /* max. safety */
        if (cache) {
            for (idx_t i = 0; i <= max_computed; ++i)
                new_arr[i] = cache[i];
            free(cache);
        }
        cache = new_arr;
        cache_cap = new_cap;
    }

    /* Initialise first six values if not yet done */
    if (max_computed < 5) {
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)9;
        cache[2] = (seq_t)20;
        cache[3] = (seq_t)44;
        cache[4] = (seq_t)96;
        cache[5] = (seq_t)209;
        max_computed = 5;
    }

    /* Compute remaining values up to 'need' */
    for (idx_t i = max_computed + 1; i <= need; ++i) {
        seq_t a1 = cache[i - 1];
        seq_t a3 = cache[i - 3];
        seq_t a6 = cache[i - 6];
        cache[i] = 3 * a1 - 4 * a3 + a6;
    }
    if (need > max_computed)
        max_computed = need;
}

seq_t a(idx_t n)
{
    ensure_cache(n);
    return cache[n];
}
