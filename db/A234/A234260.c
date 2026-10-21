/*
 * A234260  Number of (n+1) X (2+1) 0..2 arrays with every 2 X 2 subblock
 *          having its diagonal sum differing from its antidiagonal sum by 2
 *          (constant-stress 1 X 1 tilings).
 * https://oeis.org/A234260
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 6*a(n-2) - 24*a(n-3) + 4*a(n-4) + 36*a(n-5) -
 *            24*a(n-6) Initial terms: a(1)=46, a(2)=88, a(3)=170, a(4)=358,
 *            a(5)=754, a(6)=1690
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 22, u64 n <= 47, u128 n <= 97,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

#define MAX_CACHE 30000          /* sufficient for n ≤ 10000 */

static seq_t cache[MAX_CACHE + 1];
static idx_t cache_size = 0;     /* highest n stored in cache */

/* Helper to initialize the cache with the first six terms */
static void init_cache(void)
{
    cache[1] = (seq_t)46;
    cache[2] = (seq_t)88;
    cache[3] = (seq_t)170;
    cache[4] = (seq_t)358;
    cache[5] = (seq_t)754;
    cache[6] = (seq_t)1690;
    cache_size = 6;
}

seq_t a(idx_t n)
{
    if (n < (idx_t)1)            /* sequence offset is 1 */
        return (seq_t)0;

    /* Ensure cache is initialized */
    if (cache_size == 0)
        init_cache();

    /* If the requested value is already cached */
    if (n <= cache_size)
        return cache[(size_t)n];

    /* If the requested index exceeds the static cache size, fall back to
       a small sliding‑window computation. This keeps the code self‑contained
       and works even for arbitrary-precision seq_t types. */
    if (n > (idx_t)MAX_CACHE) {
        seq_t w[6] = {
            (seq_t)46,
            (seq_t)88,
            (seq_t)170,
            (seq_t)358,
            (seq_t)754,
            (seq_t)1690
        };

        if (n <= (idx_t)6)
            return w[(size_t)(n - 1)];

        for (idx_t i = (idx_t)7; i <= n; ++i) {
            seq_t nxt =
                ( (seq_t)3 * w[5] ) +            /* 3*a(i-1) */
                ( (seq_t)6 * w[4] ) +            /* 6*a(i-2) */
                ( (seq_t)-24 * w[3] ) +          /* -24*a(i-3) */
                ( (seq_t)4 * w[2] ) +            /* 4*a(i-4) */
                ( (seq_t)36 * w[1] ) +           /* 36*a(i-5) */
                ( (seq_t)-24 * w[0] );           /* -24*a(i-6) */

            /* Shift window left and insert the new term at the end */
            w[0] = w[1];
            w[1] = w[2];
            w[2] = w[3];
            w[3] = w[4];
            w[4] = w[5];
            w[5] = nxt;
        }
        return w[5];
    }

    /* Extend the cache up to n */
    for (idx_t i = cache_size + (idx_t)1; i <= n; ++i) {
        seq_t val =
            ( (seq_t)3 * cache[(size_t)(i - 1)] ) +    /* 3*a(i-1) */
            ( (seq_t)6 * cache[(size_t)(i - 2)] ) +    /* 6*a(i-2) */
            ( (seq_t)-24 * cache[(size_t)(i - 3)] ) +  /* -24*a(i-3) */
            ( (seq_t)4 * cache[(size_t)(i - 4)] ) +    /* 4*a(i-4) */
            ( (seq_t)36 * cache[(size_t)(i - 5)] ) +   /* 36*a(i-5) */
            ( (seq_t)-24 * cache[(size_t)(i - 6)] );   /* -24*a(i-6) */

        cache[(size_t)i] = val;
    }

    cache_size = n;
    return cache[(size_t)n];
}
