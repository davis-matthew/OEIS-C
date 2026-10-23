/*
 * A118334  a(n-1)*a(n-6) + a(n-2)*a(n-7) + a(n-3)*a(n-8) + a(n-4)*a(n-9),
 *          with a(k<8) = 1.
 * https://oeis.org/A118334
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 16, u32 n <= 18, u64 n <= 21, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A118334:
 *  a(0) = a(1) = … = a(7) = 1
 *  a(n) = a(n-1)*a(n-6) + a(n-2)*a(n-7) + a(n-3)*a(n-8) + a(n-4)*a(n-9)   (n ≥ 8)
 *  For n = 8 the term a(n-9) = a(-1) is defined to be 1.  */

#include <stdlib.h>
#include <string.h>

static seq_t *cache = NULL;   /* array of computed terms */
static idx_t cache_len = 0;   /* number of terms stored (next free index) */
static idx_t cache_cap = 0;   /* allocated capacity */

/* Ensure m-sized cache exists and is initialized with 1’s for indices 0‑7. */
static void init_cache(void)
{
    if (cache_len != 0)
        return;

    cache_cap = 64;                 /* initial generous capacity */
    cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    if (!cache)
        return;                     /* allocation failure → undefined behaviour */

    for (idx_t i = 0; i < 8; ++i)
        cache[i] = (seq_t)1;

    cache_len = 8;
}

/* Grow the cache so it contains terms up to and including index n. */
static void grow_cache(idx_t n)
{
    if (n < cache_len)
        return;                     /* already computed */

    /* Expand storage if necessary */
    if (cache_len + (n - cache_len + 1) > cache_cap) {
        idx_t new_cap = cache_cap * 2;
        while (new_cap <= n + 1)
            new_cap *= 2;
        seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache)
            return;                 /* allocation failure → undefined behaviour */
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        seq_t val;
        if (i == 8) {
            /* a(-1) is 1, so last product is a(4)*1 = a(4) */
            val = cache[7] * cache[2] +   /* a(7)*a(2)  */
                  cache[6] * cache[1] +   /* a(6)*a(1)  */
                  cache[5] * cache[0] +   /* a(5)*a(0)  */
                  cache[4];               /* a(4)*a(-1) = a(4) */
        } else {
            /* General recurrence for n ≥ 9 */
            val = cache[i-1] * cache[i-6] +   /* a(n-1)*a(n-6) */
                  cache[i-2] * cache[i-7] +   /* a(n-2)*a(n-7) */
                  cache[i-3] * cache[i-8] +   /* a(n-3)*a(n-8) */
                  cache[i-4] * cache[i-9];    /* a(n-4)*a(n-9) */
        }
        cache[i] = val;
    }

    cache_len = n + 1;
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n < 8)                 /* initial segment */
        return (seq_t)1;

    init_cache();
    if (n + 1 > cache_len)
        grow_cache(n);

    return cache[n];
}
