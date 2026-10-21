/*
 * A208496  Number of n X 3 0..1 arrays avoiding 0 0 0 and 0 0 1 horizontally
 *          and 0 1 0 and 1 1 1 vertically.
 * https://oeis.org/A208496
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2) + 4*a(n-3) + 2*a(n-4) + 2*a(n-5) * - 2*a(n-6) +
 *            a(n-7) + a(n-9) for n > 11 * Initial values (1‑based): * a(1)=6,
 *            a(2)=36, a(3)=60, a(4)=126, a(5)=234, * a(6)=456, a(7)=896,
 *            a(8)=1722, a(9)=3360, a(10)=6512, * a(11)=12642
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 30, u64 n <= 63, u128 n <= 130,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* A cache that grows as needed.  The element at index i holds a(i). */
static seq_t *cache = NULL;          /* cache array (index 0 unused) */
static size_t cache_size = 0;        /* highest index that the array contains */
// the largest index that has actually been computed
static size_t max_computed = 0;

/* Base values a(1) … a(11) */
static const seq_t base_vals[12] = {
    (seq_t)0,      /* base_vals[0] unused */
    (seq_t)6,      /* a(1)  */
    (seq_t)36,     /* a(2)  */
    (seq_t)60,     /* a(3)  */
    (seq_t)126,    /* a(4)  */
    (seq_t)234,    /* a(5)  */
    (seq_t)456,    /* a(6)  */
    (seq_t)896,    /* a(7)  */
    (seq_t)1722,   /* a(8)  */
    (seq_t)3360,   /* a(9)  */
    (seq_t)6512,   /* a(10) */
    (seq_t)12642   /* a(11) */
};

/* Ensure that the cache contains values up to at least 'min_needed'. */
static void ensure_min_cache(idx_t min_needed)
{
    /* min_needed is unsigned; cast safely for sizes. */
    size_t need = (size_t)min_needed;
    if (cache_size >= need)
        return;                          /* already sufficient */

    seq_t *new_cache = (seq_t *)malloc((need + 1) * sizeof(seq_t));
    if (!new_cache)
        return;                          /* allocation failure keeps old cache */

    /* copy old data */
    if (cache) {
        for (size_t i = 0; i <= cache_size; ++i)
            new_cache[i] = cache[i];
    }
    /* zero the newly allocated part */
    for (size_t i = cache_size + 1; i <= need; ++i)
        new_cache[i] = 0;

    free(cache);
    cache = new_cache;
    cache_size = need;
}

/* Compute the missing terms from (max_computed + 1) up to n */
static void compute_up_to(idx_t n)
{
    if (n <= max_computed)
        return;

    /* Ensure the array large enough. */
    ensure_min_cache(n);

    for (size_t i = max_computed + 1; i <= (size_t)n; ++i) {
        if (i <= 11) {
            /* base values already present; nothing to compute */
            /* cache[i] already set in ensure_min_cache or init below */
        } else {
            /* Recurrence: a(i) = a(i-2) + 4*a(i-3) + 2*a(i-4) + 2*a(i-5)
             *             - 2*a(i-6) + a(i-7) + a(i-9)  (for i > 11) */
            seq_t res = cache[i - 2]
                        + (seq_t)4 * cache[i - 3]
                        + (seq_t)2 * cache[i - 4]
                        + (seq_t)2 * cache[i - 5]
                        - (seq_t)2 * cache[i - 6]
                        + cache[i - 7]
                        + cache[i - 9];
            cache[i] = res;
        }
    }
    max_computed = (size_t)n;
}

/* Public function */
seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                 /* undefined for indices below offset */

    if (!cache) {
        /* Initialise cache with base values up to a(11) */
        ensure_min_cache(11);
        for (size_t i = 1; i <= 11; ++i)
            cache[i] = base_vals[i];
        max_computed = 11;
    }

    compute_up_to(n);
    return cache[(size_t)n];
}
