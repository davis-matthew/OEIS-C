/*
 * A124313  Pentanacci numbers: a(n) = a(n-1) + a(n-2) + a(n-3) + a(n-4) +
 *          a(n-5), starting 1,0,0,0,1.
 * https://oeis.org/A124313
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1)+a(n-2)+a(n-3)+a(n-4)+a(n-5); Initial values:
 *            a(1)=1, a(2)=0, a(3)=0, a(4)=0, a(5)=1
 * Range:     u8 n <= 13, u16 n <= 21, u32 n <= 38, u64 n <= 70, u128 n <= 136,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

static seq_t *pent_cache = NULL;
static size_t cache_used = 0;  /* number of values stored (>=5 after init) */
static size_t cache_cap  = 0;  /* allocated capacity */

/* Ensure the cache can store at least up to index n (1‑based). */
static void pent_ensure(idx_t n)
{
    if (pent_cache == NULL) {
        /* initial allocation for first five terms */
        cache_cap = (n > 5 ? n : 5);
        pent_cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (!pent_cache) abort();  /* out of memory */

        /* fill base cases */
        pent_cache[0] = 1;  /* a(1) */
        pent_cache[1] = 0;  /* a(2) */
        pent_cache[2] = 0;  /* a(3) */
        pent_cache[3] = 0;  /* a(4) */
        pent_cache[4] = 1;  /* a(5) */
        cache_used = 5;
    } else if ((size_t)n > cache_used) {
        /* need to grow the cache */
        size_t new_cap = cache_cap;
        while ((size_t)n > new_cap)
            new_cap = new_cap * 2 + 10; /* avoid too many reallocs */
        seq_t *tmp = (seq_t *)realloc(pent_cache, sizeof(seq_t) * new_cap);
        if (!tmp) abort();  /* out of memory */
        pent_cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = cache_used + 1; i <= n; ++i) {
        size_t idx = (size_t)(i - 1);        /* 0‑based array index */
        seq_t sum =
            pent_cache[idx - 1] +
            pent_cache[idx - 2] +
            pent_cache[idx - 3] +
            pent_cache[idx - 4] +
            pent_cache[idx - 5];
        pent_cache[idx] = sum;
    }
    if ((size_t)n > cache_used)
        cache_used = (size_t)n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;  /* undefined, but avoid crash */

    pent_ensure(n);
    return pent_cache[(size_t)(n - 1)];
}
