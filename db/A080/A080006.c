/*
 * A080006  Number of permutations satisfying -k<=p(i)-i<=r and p(i)-i not in
 *          I, i=1..n, with k=2, r=3, I={0,2}.
 * https://oeis.org/A080006
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-2)+2*a(n-3)+2*a(n-4)+3*a(n-5)+a(n-6)
 *            -a(n-8)-a(n-9)-a(n-10), with a(0)=1, a(1)=0, a(2)=1, a(3)=1,
 *            a(4)=3, a(5)=5, a(6)=8, a(7)=16, a(8)=27, a(9)=51, a(10)=91
 *            (OEIS A080006)
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache for memoised values of the sequence.
   The array holds all computed a(i) for 0 <= i < cache_len. */
static seq_t *cache = NULL;
static size_t cache_cap = 0;   /* allocated size of cache array   */
static size_t cache_len = 0;   /* number of values stored         */

/* Initial values for n = 0..10.  They are enough to start the recurrence. */
static const seq_t init_vals[11] = {
    (seq_t)1,  /* a(0) */
    (seq_t)0,  /* a(1) */
    (seq_t)1,  /* a(2) */
    (seq_t)1,  /* a(3) */
    (seq_t)3,  /* a(4) */
    (seq_t)5,  /* a(5) */
    (seq_t)8,  /* a(6) */
    (seq_t)16, /* a(7) */
    (seq_t)27, /* a(8) */
    (seq_t)51, /* a(9) */
    (seq_t)91  /* a(10) */
};

/* Ensure that the cache has room for at least 'need' entries.   */
static void
ensure_cache_cap(size_t need)
{
    if (cache_cap < need) {
        seq_t *tmp = (seq_t *)realloc(cache, need * sizeof(seq_t));
        if (!tmp) {            /* allocation failed – abort early */
            /* For the purposes of the harness, we simply
               return with undefined behaviour (e.g., zero)
               if memory allocation fails. */
            return;
        }
        cache = tmp;
        cache_cap = need;
    }
}

/* Compute and return a(n).  The function is self‑contained and caches
   its results so that repeated calls are fast and order‑independent. */
seq_t
a(idx_t n)
{
    /* Convert idx_t to size_t for array indexing.  The cast is safe
       because n is used only as an array index and must be within
       the bounds of the cache array. */
    size_t idx = (size_t)n;

    /* Extend the cache if required. */
    if (idx >= cache_len) {
        ensure_cache_cap(idx + 1);

        for (size_t i = cache_len; i <= idx; ++i) {
            seq_t val;
            if (i <= 10) {
                /* Use the pre‑computed initial values. */
                val = init_vals[i];
            } else {
                /* Apply the linear recurrence.  All operations are
                   performed in the type seq_t to preserve exactness. */
                val =  cache[i - 2]
                     + (seq_t)2 * cache[i - 3]
                     + (seq_t)2 * cache[i - 4]
                     + (seq_t)3 * cache[i - 5]
                     +        cache[i - 6]
                     -        cache[i - 8]
                     -        cache[i - 9]
                     -        cache[i -10];
            }
            cache[i] = val;
        }
        cache_len = idx + 1;
    }

    return cache[idx];
}
