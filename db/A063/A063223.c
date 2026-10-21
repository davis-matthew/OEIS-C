/*
 * A063223  Dimension of the space of weight 2n cuspidal newforms for Gamma_0(
 *          59 ).
 * https://oeis.org/A063223
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = a(n-2)+a(n-3)-a(n-5) (n > 6), a(1) = 5, a(2) = 14, a(3) =
 *            24, a(4) = 34, a(5) = 44, a(6) = 52
 * Range:     u8 n <= 26, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* memoization buffer, cache[0] unused */
static idx_t  cache_cap = 0;  /* allocated size (in terms of array index) */
static idx_t  cache_max = 0;  /* highest index that has been computed */

seq_t a(idx_t n)
{
    /* Ensure we have enough space for index n */
    if (n + 1 > cache_cap) {
        idx_t new_cap = n + 1;
        if (new_cap < 16) new_cap = 16;          /* minimal grow */
        /* realloc returns void*, cast to seq_t* for both C and C++ */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                      /* cannot allocate */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* If the value already computed, return it */
    if (n <= cache_max)
        return cache[n];

    /* Compute all missing values up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        if (i == 1)       cache[i] = 5;
        else if (i == 2)  cache[i] = 14;
        else if (i == 3)  cache[i] = 24;
        else if (i == 4)  cache[i] = 34;
        else if (i == 5)  cache[i] = 44;
        else if (i == 6)  cache[i] = 52;
        else              cache[i] = cache[i-2] + cache[i-3] - cache[i-5];
    }
    cache_max = n;
    return cache[n];
}
