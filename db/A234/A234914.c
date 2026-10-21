/*
 * A234914  Number of (n+1) X (1+1) 0..4 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 2,
 *          with no adjacent elements equal (constant-stress tilted 1 X 1
 *          tilings).
 * https://oeis.org/A234914
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - 2*a(n-2) - 2*a(n-3) + a(n-4)
 * Range:     u8 n <= 2, u16 n <= 7, u32 n <= 16, u64 n <= 35, u128 n <= 73,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*           a(1) =  40,  a(2) = 120,  a(3) = 376,  a(4) = 1200  */
#include <stdlib.h>
#include <stddef.h>

static seq_t *cache     = NULL;   /* 1‑based cache: cache[0] unused */
static size_t cache_len = 0;      /* largest n for which cache[n] is valid */
static size_t cache_cap = 0;      /* allocated capacity */

/* Ensure the cache can address indices up to n (inclusive) */
static void ensure_cache_size(idx_t n)
{
    if ((size_t)(n + 1) <= cache_cap)
        return;
    size_t new_cap = (size_t)(n + 1);
    if (new_cap < 16)
        new_cap = 16;
    void *p = realloc(cache, new_cap * sizeof(seq_t));
    if (!p)
        abort();                  /* unlikely on an online judge */
    cache = (seq_t *)p;
    cache_cap = new_cap;
}

/* Sequence function */
seq_t a(idx_t n)
{
    if (n == (idx_t)0)        /* offset starts at 1; behaviour for 0 is undefined */
        return (seq_t)0;

    ensure_cache_size(n);

    /* Return previously cached value if available. */
    if (n <= cache_len)
        return cache[n];

    /* Initialise the first four terms once. */
    if (cache_len == 0) {
        cache[0] = (seq_t)0;          /* unused slot */
        cache[1] = (seq_t)40;
        cache[2] = (seq_t)120;
        cache[3] = (seq_t)376;
        cache[4] = (seq_t)1200;
        cache_len = 4;
    }

    /* Compute missing terms up to n.  Only run if n exceeds cache_len. */
    if (n > cache_len) {
        for (idx_t i = cache_len + 1; i <= n; ++i) {
            seq_t val = 4 * cache[i - 1];
            val -= 2 * cache[i - 2];
            val -= 2 * cache[i - 3];
            val += cache[i - 4];
            cache[i] = val;
        }
        cache_len = n;
    }

    return cache[n];
}
