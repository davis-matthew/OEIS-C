/*
 * A111991  Second convolution of A111989.
 * https://oeis.org/A111991
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*a(n-1) - 108*a(n-2) + 192*a(n-3) + 288*a(n-4) -
 *            864*a(n-5) - 192*a(n-6) + 1152*a(n-7) - 512*a(n-9) with initial
 *            values a(0)=1, a(1)=18, a(2)=216, a(3)=2136, a(4)=18864,
 *            a(5)=154656, a(6)=1203072, a(7)=9000576, a(8)=65339136
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 21, u128 n <= 46,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 188 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* helper to allocate an array of seq_t that works in both C and C++ */
static inline seq_t *seq_malloc(idx_t n)
{
#ifdef __cplusplus
    return mem_new(seq_t, n);
#else
    return (seq_t *)malloc(n * sizeof(seq_t));
#endif
}

static inline seq_t *seq_realloc(seq_t *p, idx_t n)
{
#ifdef __cplusplus
    return mem_resize(seq_t, p, n);
#else
    return (seq_t *)realloc(p, n * sizeof(seq_t));
#endif
}

/* Static cache for computed terms */
static seq_t *cache = NULL;        /* pointer to array of terms */
static idx_t cache_len = 0;        /* number of terms stored (cache_len > 0 if cached) */

/* Initialize cache with the first nine terms */
static void init_cache(void)
{
    if (cache == NULL)
    {
        cache_len = 9;
        cache = seq_malloc(cache_len);   /* allocate array of seq_t */
        /* Base terms are hard‑coded from the OEIS data */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)18;
        cache[2] = (seq_t)216;
        cache[3] = (seq_t)2136;
        cache[4] = (seq_t)18864;
        cache[5] = (seq_t)154656;
        cache[6] = (seq_t)1203072;
        cache[7] = (seq_t)9000576;
        cache[8] = (seq_t)65339136;
    }
}

/* Compute a(n) using the recurrence, extending cache as needed */
seq_t a(idx_t n)
{
    init_cache();                       /* ensure cache is ready */

    /* Extend cache up to n if necessary */
    if (n >= cache_len)
    {
        /* Allocate space for all needed terms */
        seq_t *new_cache = seq_realloc(cache, n + 1);
        cache = new_cache;
        /* Compute terms from current cache_len up to n */
        for (idx_t i = cache_len; i <= n; ++i)
        {
            /* Positive contributions */
            seq_t pos = 18 * cache[i-1];
            pos += 192 * cache[i-3];
            pos += 288 * cache[i-4];
            pos += 1152 * cache[i-7];

            /* Negative contributions */
            seq_t neg = 108 * cache[i-2];
            neg += 864 * cache[i-5];
            neg += 192 * cache[i-6];
            neg += 512 * cache[i-9];

            cache[i] = pos - neg;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
