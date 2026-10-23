/*
 * A186605  Number of (n+4)X9 binary arrays with every 5X5 subblock commuting
 *          with each horizontal and vertical neighbor 5X5 subblock.
 * https://oeis.org/A186605
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Empirical recurrence:
 *            a(n)=a(n-1)+a(n-3)-a(n-4)+a(n-5)-a(n-6)-a(n-8)+a(n-9)+a(n-10)-a(n-13)
 *            for n>19. Initial values: a(1)=156276, a(2)=10882, a(3)=19840,
 *            a(4)=54040, a(5)=101080, a(6)=126344, a(7)=132040, a(8)=133484,
 *            a(9)=134936, a(10)=136416, a(11)=138160, a(12)=141124,
 *            a(13)=149504, a(14)=175...
 * Range:     u8 none, u16 none, u32 n <= 60, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

/* initial terms a(1) … a(19) */
static const seq_t init_vals[19] = {
    (seq_t)156276,  /* a(1) */
    (seq_t)10882,   /* a(2) */
    (seq_t)19840,   /* a(3) */
    (seq_t)54040,   /* a(4) */
    (seq_t)101080,  /* a(5) */
    (seq_t)126344,  /* a(6) */
    (seq_t)132040,  /* a(7) */
    (seq_t)133484,  /* a(8) */
    (seq_t)134936,  /* a(9) */
    (seq_t)136416,  /* a(10) */
    (seq_t)138160,  /* a(11) */
    (seq_t)141124,  /* a(12) */
    (seq_t)149504,  /* a(13) */
    (seq_t)175064,  /* a(14) */
    (seq_t)237212,  /* a(15) */
    (seq_t)343672,  /* a(16) */
    (seq_t)478073,  /* a(17) */
    (seq_t)619334,  /* a(18) */
    (seq_t)779227   /* a(19) */
};

/* cache of computed terms; cache[i] holds a(i+1) */
static seq_t *cache = NULL;
static size_t cache_len = 0;   /* number of terms currently stored */

/* ensure that cache contains terms up to and including n */
static void ensure_cache(idx_t n)
{
    if (n == 0)
        return;                /* undefined for n < 1 */

    /* initialize cache with the 19 seed values on first call */
    if (cache == NULL) {
        cache_len = sizeof(init_vals) / sizeof(init_vals[0]);   /* 19 */
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache)
            return;           /* allocation failed – caller will treat as 0 */
        for (size_t i = 0; i < cache_len; ++i)
            cache[i] = init_vals[i];
    }

    if ((size_t)n <= cache_len)
        return;                /* already computed */

    /* Grow the allocated array at least to size n; use exponential growth */
    size_t new_len = cache_len;
    while (new_len < (size_t)n)
        new_len <<= 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
    if (!tmp)
        return;               /* allocation failed – cache remains unchanged */

    cache = tmp;
    /* compute missing terms using the recurrence; indices are zero‑based:
       cache[i] = a(i+1) */
    for (size_t i = cache_len; i < (size_t)n; ++i)
    {
        seq_t val = cache[i - 1];          /* a(i)  = a(i)   of recurrence */
        val += cache[i - 3];               /* a(i+2)    */
        val -= cache[i - 4];
        val += cache[i - 5];
        val -= cache[i - 6];
        val -= cache[i - 8];
        val += cache[i - 9];
        val += cache[i - 10];
        val -= cache[i - 13];
        cache[i] = val;
    }
    cache_len = n;
}

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;   /* undefined for n < 1 */

    ensure_cache(n);
    if (cache == NULL || n > cache_len)
        return (seq_t)0;   /* allocation failed */

    return cache[n - 1];
}
