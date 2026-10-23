/*
 * A199641  Number of n X 2 0..2 arrays with values 0..2 introduced in row
 *          major order and each element equal to one or two horizontal and
 *          vertical neighbors.
 * https://oeis.org/A199641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 29, u128 n <= 57,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Method: Empirical linear recurrence
   a(1)=1, a(2)=3, a(3)=13, a(4)=60, a(5)=288, a(6)=1384, a(7)=6628
   For n>7: a(n)=3*a(n-1)+7*a(n-2)+8*a(n-3)-8*a(n-5)  */
/* Note: values grow quickly; implementation uses memoization and resizes a cache. */

#include <stdlib.h>

/* Forward declaration of the function */
seq_t a(idx_t n);

/* Global cache for computed values */
static seq_t *cache = NULL;      /* pointer to array of seq_t */
static size_t cache_cap = 0;     /* allocated capacity */
static idx_t cache_len = 0;      /* number of values stored (cache_len >= 0) */

/* Ensure the cache can hold at least `need` elements (1‑based). */
static void ensure_cache(idx_t need)
{
    /* Convert to 1‑based requirement, array is 0‑based */
    if ((idx_t)cache_cap >= need)
        return;
    /* Grow capacity: at least double, but at most need */
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    if (new_cap < (size_t)need)
        new_cap = (size_t)need;
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) {
        /* Allocation failure: abort as behavior undefined per problem constraints */
        abort();
    }
    cache = tmp;
    cache_cap = new_cap;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n < 1)        /* undefined for n==0, but return 0 */
        return (seq_t)0;

    /* Grow cache if necessary */
    if ((idx_t)cache_len < n) {
        /* Ensure capacity for needed size */
        ensure_cache(n);
        /* Initialize base cases if not already present */
        if (cache_len == 0) {
            cache[0] = (seq_t)1;   /* a(1) */
            cache[1] = (seq_t)3;   /* a(2) */
            cache[2] = (seq_t)13;  /* a(3) */
            cache[3] = (seq_t)60;  /* a(4) */
            cache[4] = (seq_t)288; /* a(5) */
            cache[5] = (seq_t)1384;/* a(6) */
            cache[6] = (seq_t)6628;/* a(7) */
            cache_len = 7;
        }
        /* Compute remaining values up to n */
        for (idx_t k = cache_len + 1; k <= n; ++k) {
            if (k <= 7) {
                /* Already initialized */
                continue;
            }
            /* Apply recurrence: a(k)=3*a(k-1)+7*a(k-2)+8*a(k-3)-8*a(k-5) */
            seq_t v = (seq_t)3 * cache[k - 2]     /* 3*a(k-1) */
                    + (seq_t)7 * cache[k - 3]     /* 7*a(k-2) */
                    + (seq_t)8 * cache[k - 4]     /* 8*a(k-3) */
                    - (seq_t)8 * cache[k - 6];    /* -8*a(k-5) */
            cache[k - 1] = v;
        }
        cache_len = n;
    }
    return cache[n - 1];
}
