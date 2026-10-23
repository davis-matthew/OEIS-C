/*
 * A170725  Number of reduced words of length n in Coxeter group on 44
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^50 = I.
 * https://oeis.org/A170725
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A170725: number of reduced words of length n in the Coxeter
   group on 44 generators S_i with relations (S_i)^2 = (S_i S_j)^50 = I.
   The sequence satisfies the linear recurrence
        a(0) = 1
        a(n) = 2 + 42·∑_{k=1}^{n} a(n–k)            for 1 ≤ n ≤ 49
        a(50) = 1 + 42·∑_{k=1}^{49} a(50–k) – 903·a(0)
        a(n) = 42·∑_{k=1}^{49} a(n–k) – 903·a(n–50)   for n > 50
   (which is equivalent to the recurrence derived from the rational
   generating function given in the OEIS entry).  The function below
   memoises previously computed values, so calls can arrive in any order. */

#include <stddef.h>
#include <stdlib.h>

extern idx_t _floor_isqrt_u64(idx_t n);   /* dummy prototype to avoid warnings */

/* cache of computed terms */
static seq_t *cache     = NULL;   /* array a[0..cache_cap-1]            */
static idx_t cache_cap  = 0;      /* current allocated size             */
static idx_t cache_done = 0;      /* number of values computed (>=0)    */

/* ensure that the cache can hold indices up to 'n' */
static void ensure_cache(idx_t n)
{
    if (cache == NULL) {
        cache_cap  = n + 1;
        cache      = (seq_t *)malloc((size_t)cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache_done = 0;
    } else if (n + 1 > cache_cap) {
        idx_t new_cap  = n + 1;
        seq_t *new_mem = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (!new_mem) abort();
        cache      = new_mem;
        cache_cap  = new_cap;
        /* cache_done remains unchanged */
    }
}

/* main sequence function */
seq_t a(idx_t n)
{
    ensure_cache(n);

    /* compute any missing values */
    while (cache_done <= n) {
        seq_t val;
        if (cache_done == 0) {
            val = (seq_t)1;
        } else if (cache_done < 50) {
            /* a(n) = 2 + 42 * sum_{k=1}^{n} a(n-k)   (1 ≤ n ≤ 49) */
            seq_t sum = 0;
            for (idx_t k = 1; k <= cache_done; ++k)
                sum += cache[cache_done - k];
            val = (seq_t)2 + (seq_t)42 * sum;
        } else if (cache_done == 50) {
            /* a(50) = 1 + 42 * sum_{k=1}^{49} a(50-k) – 903·a(0) */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 49; ++k)
                sum += cache[50 - k];
            seq_t prod1 = (seq_t)42 * sum;
            seq_t prod2 = (seq_t)903 * cache[0];
            val = (seq_t)1 + prod1;
            if (val < prod2) abort();   /* should not happen */
            val -= prod2;
        } else {  /* cache_done > 50 */
            /* a(n) = 42 * sum_{k=1}^{49} a(n-k) – 903 * a(n-50) */
            seq_t sum = 0;
            for (idx_t k = 1; k <= 49; ++k)
                sum += cache[cache_done - k];
            seq_t prod1 = (seq_t)42 * sum;
            seq_t prod2 = (seq_t)903 * cache[cache_done - 50];
            if (prod1 < prod2) abort();   /* should not happen */
            val = prod1 - prod2;
        }

        cache[cache_done] = val;
        ++cache_done;
    }

    return cache[n];
}
