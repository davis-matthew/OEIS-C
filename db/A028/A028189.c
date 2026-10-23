/*
 * A028189  Expansion of 1/((1-5*x)*(1-7*x)*(1-11*x)*(1-12*x)).
 * https://oeis.org/A028189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7^(n+1)-5^(n+1))/2 + 23*a(n-1) - 132*a(n-2); a(0) = 1,
 *            a(1) = 35
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 16, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Helper to grow a dynamic array of seq_t values */
static void ensure_capacity(idx_t n)
{
    extern seq_t *cache;      /* defined elsewhere */
    static size_t cache_cap = 0;
}

/* Fenwick or similar.  The actual storage and sizes are managed
   statically within the function below.  */

seq_t a(idx_t n)
{
    /* Static storage for computed values and auxiliary data */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_sz  = 0;    /* number of terms stored */
    static seq_t pow5 = 0;          /* 5^(current_index+1) for next term */
    static seq_t pow7 = 0;          /* 7^(current_index+1) for next term */
    static idx_t max_done = 0;      /* highest index that has been computed */

    /* Lazy initialization */
    if (!cache) {
        cache_cap = 2;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)35;
        cache_sz = 2;
        max_done = 1;
        /* powers needed for computing a(2) :
           we keep 5^(current_index+1) for the *next* index to compute.
           At start, the next index will be 2, so we set them to 5^2 and 7^2. */
        pow5 = (seq_t)5 * (seq_t)5;   /* 25 */
        pow7 = (seq_t)7 * (seq_t)7;   /* 49 */
    }

    if (n < cache_sz)
        return cache[n];

    /* Grow the array as necessary */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 2;
        while (new_cap < (size_t)(n + 1))
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute terms iteratively up to n */
    for (idx_t i = max_done + 1; i <= n; ++i) {
        /* Update powers to 5^(i+1) and 7^(i+1) */
        pow5 = pow5 * (seq_t)5;
        pow7 = pow7 * (seq_t)7;

        /* term = (7^(i+1) - 5^(i+1)) / 2 */
        seq_t term = (pow7 - pow5) / (seq_t)2;

        /* recurrence: a(i) = 23*a(i-1) - 132*a(i-2) + term */
        seq_t val = cache[i - 1] * (seq_t)23;
        val -= cache[i - 2] * (seq_t)132;
        val += term;

        cache[i] = val;
    }

    cache_sz = n + 1;
    max_done = n;
    return cache[n];
}
