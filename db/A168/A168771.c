/*
 * A168771  Number of reduced words of length n in Coxeter group on 46
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^18 = I.
 * https://oeis.org/A168771
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Recurrence: 
   a(0) = 1
   For n ≥ 1 
   a(n) = 44 * Σ_{k=1}^{min(17,n)} a(n-k) – 990 * a(n-18) + 
          { 1, if n = 0 or n = 18
            2, if 1 ≤ n ≤ 17
            0, otherwise }.
   This follows from the rational generating function  
   (t^18 + 2t^17 + … + 2t + 1) /
   (990t^18 – 44t^17 – … – 44t + 1).
*/

#include <stdlib.h>

/* Persistent cache for computed terms (O(1) per term after first pass). */
static seq_t *cache = NULL;          /* array of a(i) for i < cache_len   */
static idx_t   cache_len = 0;        /* number of terms currently stored */
static size_t  cache_cap = 0;        /* allocated capacity of the array   */

/* Ensure the cache can hold indices up to n (inclusive). */
static void ensure_capacity(idx_t n)
{
    if (cache == NULL) {                /* first allocation           */
        cache_cap = 64;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    }
    while (cache_cap <= n) {
        size_t new_cap = cache_cap * 2;
        if (new_cap == 0) new_cap = 16; /* safety for rare zero case  */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
}

/* Compute the nth term of the sequence. */
seq_t a(idx_t n)
{
    /* Initialise first term when cache is empty. */
    if (cache == NULL) {
        ensure_capacity(0);
        cache[0] = 1;    /* a(0) = 1 */
        cache_len = 1;
    }

    /* Compute terms up to n if not already cached. */
    if (n + 1 > cache_len) {
        ensure_capacity(n);
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t sum = 0;
            idx_t k;
            /* Sum of the previous up to 17 terms. */
            for (k = 1; k <= 17 && k <= i; ++k)
                sum += cache[i - k];
            seq_t val = sum * (seq_t)44;
            if (i >= 18)
                val -= cache[i - 18] * (seq_t)990;

            /* Add the polynomial part from the numerator. */
            if (i == 0)
                val += 1;
            else if (i == 18)
                val += 1;
            else if (i <= 17)
                val += 2;   /* for 1 <= i <= 17 */

            cache[i] = val;
        }
        cache_len = n + 1;
    }

    return cache[n];
}
