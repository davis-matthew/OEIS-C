/*
 * A236148  Number of (n+1) X (1+1) 0..2 arrays colored with the sets of
 *          distinct values in every 2 X 2 subblock.
 * https://oeis.org/A236148
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + 18*a(n-2) + 16*a(n-3) + 12*a(n-4) - 12*a(n-5)
 *            Initial values for n = 1..5: 81, 414, 2148, 11064, 57132
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 11, u64 n <= 25, u128 n <= 52,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  This implementation uses memoised recurrence evaluation.
    It works for any build where seq_t and idx_t are standard
    unsigned integer types or a big‑int class.                                         */

#include <stddef.h>   /* size_t */
#include <stdlib.h>   /* malloc, realloc, calloc, free */

static const seq_t init_vals[5] = {
    (seq_t)81,    (seq_t)414,    (seq_t)2148,
    (seq_t)11064, (seq_t)57132
};

static seq_t *cache      = NULL;   /* 1‑based cache: cache[1] == a(1) */
static size_t cache_cap  = 0;       /* number of allocated slots (including index 0) */
static idx_t  last_valid = 0;       /* highest n whose value is stored in cache */

/* Ensure cache can hold terms up to n (inclusive). */
static void ensure_cache(idx_t n)
{
    /* n is 1‑based; convert to size_t for capacity. */
    size_t need = (size_t)n + 1;
    if (need <= cache_cap)
        return;

    /* Allocate (or reallocate) the cache array. */
    seq_t *tmp = (cache == NULL)
        ? (seq_t*)calloc(need, sizeof(seq_t))
        : (seq_t*)realloc(cache, need * sizeof(seq_t));

    if (tmp == NULL) {
        /* Allocation failed; terminate the program. */
        abort();
    }
    cache = tmp;
    /* If we grew the array, new elements remain zeroed by calloc/realloc. */
    cache_cap = need;
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;                     /* undefined for n < 1 */

    if (n <= 5)
        return init_vals[(size_t)n - 1];     /* base cases */

    /* Initialise cache if it hasn't been yet.  The first five slots are filled. */
    if (cache == NULL) {
        /* Allocate a modest initial size to avoid many reallocs for small n. */
        size_t init_cap = (size_t)(n > 16 ? 16 : n + 1);
        cache      = (seq_t*)calloc(init_cap, sizeof(seq_t));
        if (cache == NULL) abort();
        for (size_t i = 1; i <= 5; ++i)
            cache[i] = init_vals[i - 1];
        cache_cap  = init_cap;
        last_valid = 5;
    }

    /* Extend cache to at least n, computing missing terms. */
    if (n > last_valid) {
        /* Ensure the array is large enough. */
        ensure_cache(n);

        /* Compute terms from last_valid+1 up to n. */
        for (idx_t i = last_valid + 1; i <= n; ++i) {
            /* Recurrence: a(i) = a(i-1) + 18*a(i-2) + 16*a(i-3)
               + 12*a(i-4) - 12*a(i-5) */
            seq_t val = cache[(size_t)(i-1)]
                      + (seq_t)18 * cache[(size_t)(i-2)]
                      + (seq_t)16 * cache[(size_t)(i-3)]
                      + (seq_t)12 * cache[(size_t)(i-4)]
                      - (seq_t)12 * cache[(size_t)(i-5)];
            cache[(size_t)i] = val;
        }
        last_valid = n;
    }

    return cache[(size_t)n];
}
