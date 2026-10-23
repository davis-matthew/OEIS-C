/*
 * A199849  Number of -n..n arrays x(0..4) of 5 elements with zero sum and no
 *          element more than one greater than the previous.
 * https://oeis.org/A199849
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-3) - 2*a(n-5) + 2*a(n-6) + a(n-8) -
 *            2*a(n-10) + a(n-11) for n > 12 Initial values: a(1)=26, a(2)=66,
 *            a(3)=132, a(4)=239, a(5)=399, a(6)=630, a(7)=944, a(8)=1367,
 *            a(9)=1913, a(10)=2612, a(11)=3482, a(12)=4557 The function
 *            memoizes results so repeated calls are O(1)
 * Range:     u8 n <= 4, u16 n <= 26, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static memoization buffer */
static seq_t *cache     = NULL;  /* array of cached values, 1‑based */
static idx_t  cache_sz  = 0;     /* allocated size (index+1)    */
static idx_t  computed  = 0;     /* largest index computed      */

/* Initial terms a(1) … a(12) */
static const seq_t init_vals[13] = {
    0,    /* unused 0‑th entry */
    26,   66,   132,   239,   399,
    630,  944,  1367,  1913,  2612,
    3482, 4557
};

/* Ensure the cache holds at least indices up to n (including 0…n). */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        /* First allocation: enough for 12 terms and up to n. */
        cache_sz = (n > 12 ? n : 12) + 1;
        cache = (seq_t *)malloc(cache_sz * sizeof(seq_t));
        if (!cache) return;  /* malloc failure – return 0 later */
        /* Fill known initial terms */
        for (idx_t i = 1; i <= 12; ++i) cache[i] = init_vals[i];
        computed = 12;
    } else if (n + 1 > cache_sz) {
        /* Grow the buffer */
        cache_sz = n + 1;
        cache = (seq_t *)realloc(cache, cache_sz * sizeof(seq_t));
        /* new slots will be filled when needed */
    }
    /* If cache already big enough – nothing to do */
}

/* Main sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                     /* not defined, but safe */
    if (n <= 12) return init_vals[n];         /* return table entry */

    ensure_cache(n);                          /* guarantee space up to n */

    /* Compute missing values sequentially, reusing earlier ones */
    for (idx_t i = computed + 1; i <= n; ++i) {
        /* Order the operations to avoid underflow on unsigned seq_t */
        seq_t pos = (cache[i-1] * 2)     /* 2*a(i-1) */
                 + (cache[i-6] * 2)     /* 2*a(i-6) */
                 + cache[i-8]           /* a(i-8)   */
                 + cache[i-11];         /* a(i-11)  */

        seq_t neg = cache[i-3]          /* a(i-3)   */
                 + (cache[i-5] * 2)     /* 2*a(i-5) */
                 + (cache[i-10] * 2);   /* 2*a(i-10)*/

        cache[i] = pos - neg;
    }

    computed = n;
    return cache[n];
}
