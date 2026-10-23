/*
 * A197057  Number of n X 4 0..4 arrays with each element equal to the number
 *          its horizontal and vertical neighbors equal to 2.
 * https://oeis.org/A197057
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-1) + a(n-3) + 2*a(n-4) + 3*a(n-5) + 2*a(n-6) - a(n-7)
 *            - 3*a(n-8) - 2*a(n-9), with a(1)=1, a(2)=4, a(3)=10, a(4)=18,
 *            a(5)=28, a(6)=47, a(7)=93, a(8)=189, a(9)=357. See OEIS A197057
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 71, u128 n <= 142,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Memoized recursive implementation based on the empirical recurrence. */
static seq_t *cache = NULL;          /* Array of computed terms, 1-indexed. */
static idx_t cache_cap = 0;          /* Allocated capacity of cache. */
static idx_t cache_len = 0;          /* Largest n for which cache[n] is valid. */

/* Initial known values for n = 1..9. */
static const seq_t base[10] = {
    0ULL,          /* index 0 unused */
    1ULL,          /* a(1) */
    4ULL,          /* a(2) */
    10ULL,         /* a(3) */
    18ULL,         /* a(4) */
    28ULL,         /* a(5) */
    47ULL,         /* a(6) */
    93ULL,         /* a(7) */
    189ULL,        /* a(8) */
    357ULL         /* a(9) */
};

seq_t a(idx_t n) {
    if (n <= 9ULL)                /* Direct lookup for the first nine terms */
        return base[n];

    /* Initialise the cache on the first call. */
    if (cache == NULL) {
        cache_cap = 16ULL;                            /* Start with some capacity */
        cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
    }

    /* Store the known first nine terms if not already stored. */
    if (cache_len < 9ULL) {
        /* Ensure the cache covers indices 1..9. */
        if (cache_cap < 9ULL) {
            size_t new_cap = 9ULL;
            cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
            cache_cap = new_cap;
        }
        for (idx_t i = 1ULL; i <= 9ULL; ++i)
            cache[i] = base[i];
        cache_len = 9ULL;
    }

    /* Grow the cache if necessary. */
    while (cache_cap < n) {
        idx_t new_cap = cache_cap * 2ULL;
        if (new_cap < n)
            new_cap = n;                   /* Ensure we can store up to n */
        cache = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* Compute terms from cache_len+1 up to n. */
    for (idx_t i = cache_len + 1ULL; i <= n; ++i) {
        seq_t val =
            cache[i - 1ULL]                  /* a(i-1) */
          + cache[i - 3ULL]                  /* a(i-3) */
          + 2 * cache[i - 4ULL]              /* 2*a(i-4) */
          + 3 * cache[i - 5ULL]              /* 3*a(i-5) */
          + 2 * cache[i - 6ULL]              /* 2*a(i-6) */
          - cache[i - 7ULL]                  /* -a(i-7) */
          - 3 * cache[i - 8ULL]              /* -3*a(i-8) */
          - 2 * cache[i - 9ULL];             /* -2*a(i-9) */
        cache[i] = val;
    }
    cache_len = n;

    return cache[n];
}
