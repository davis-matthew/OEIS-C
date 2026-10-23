/*
 * A197051  Number of nX5 0..4 arrays with each element equal to the number
 *          its horizontal and vertical zero neighbors.
 * https://oeis.org/A197051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 76,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  
 * OEIS sequence A197051  
 * Formula: a(n) = a(n-1) +4*a(n-2) +10*a(n-3) +4*a(n-4) -20*a(n-5) +a(n-6) -2*a(n-7) +2*a(n-8) -16*a(n-9) +4*a(n-10) +a(n-13)
 * Initial values (n = 1 … 14) are given below.
 */

#include <stdlib.h>
#include <stddef.h>

static const seq_t init_vals[14] = {
    (seq_t)4,      (seq_t)10,     (seq_t)38,     (seq_t)108,
    (seq_t)358,    (seq_t)1132,   (seq_t)3580,   (seq_t)11382,
    (seq_t)36270,  (seq_t)114992, (seq_t)365628, (seq_t)1162290,
    (seq_t)3692624,(seq_t)11733828
};

/* cache[i] = a(i+1)  for 0 <= i < cache_size */
static seq_t *cache      = NULL;
static size_t cache_cap  = 0;
static size_t cache_size = 0;   /* number of terms currently stored */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined, return 0 */

    /* Return precomputed value for the first 14 terms */
    if (n <= 14) return init_vals[n-1];

    /* Ensure the cache has enough capacity for n terms */
    if (!cache || cache_cap < (size_t)n) {
        size_t new_cap = cache_cap ? cache_cap : 64;
        if (new_cap < 14) new_cap = 14;
        while (new_cap < (size_t)n) new_cap <<= 1;
        void *tmp = realloc(cache, new_cap * sizeof(*cache));
        if (!tmp) return (seq_t)0;          /* allocation failure – unlikely */
        cache = (seq_t*)tmp;
        cache_cap = new_cap;
    }

    /* Initialise the first 14 entries of the cache, if not already done */
    if (cache_size < 14) {
        for (idx_t i = cache_size + 1; i <= 14; ++i)
            cache[i-1] = init_vals[i-1];
        cache_size = 14;
    }

    /* Compute terms from cache_size+1 up to n */
    for (idx_t i = (idx_t)cache_size + 1; i <= n; ++i) {
        /* Positive contributions */
        seq_t pos = cache[i-2]                           /* a(n-1) */
                 + (seq_t)4 * cache[i-3]                 /* 4*a(n-2) */
                 + (seq_t)10 * cache[i-4]                /* 10*a(n-3) */
                 + (seq_t)4 * cache[i-5]                 /* 4*a(n-4) */
                 + cache[i-7]                            /* + a(n-6) */
                 + (seq_t)2 * cache[i-9]                 /* + 2*a(n-8) */
                 + (seq_t)4 * cache[i-11]                /* + 4*a(n-10) */
                 + cache[i-14];                         /* + a(n-13) */

        /* Negative contributions */
        seq_t neg = (seq_t)20 * cache[i-6]                 /* -20*a(n-5) */
                 + (seq_t)2 * cache[i-8]                  /* -2*a(n-7) */
                 + (seq_t)16 * cache[i-10];              /* -16*a(n-9) */

        cache[i-1] = pos - neg;
    }

    cache_size = (size_t)n;
    return cache[n-1];
}
