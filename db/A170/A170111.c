/*
 * A170111  Number of reduced words of length n in Coxeter group on 6
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^38 = I.
 * https://oeis.org/A170111
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A170111: Number of reduced words of length n in the Coxeter
 * group on 6 generators with relations (S_i)^2 = (S_i S_j)^38 = I.
 *
 * Recurrence:
 *   a(0) = 1
 *   For 1 <= n < 38   : a(n) = 6 * 5^(n-1)
 *   For n >= 38       : a(n) = 4 * (a(n-1)+a(n-2)+...+a(n-37)) - 10 * a(n-38)
 *
 * The function below computes a(n) for any n >= 0, memoising previously
 * computed results in a static dynamic array.  All arithmetic is done with
 * the generic integer type `seq_t`, which supports <<, +, -, *, etc.
 */

#include <stdlib.h>
#include <string.h>   /* for memcpy */

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache = NULL;      /* array of computed values */
    static idx_t  cache_size = 0;    /* allocated size of cache */
    static idx_t  max_computed = 0;  /* largest index whose value is stored */

    /* Allocate or grow cache to hold index n */
    if (cache == NULL) {
        cache_size = n + 1;
        cache = (seq_t *)malloc((size_t)cache_size * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* allocation failure – unlikely */
    } else if ((idx_t)n + 1 > cache_size) {
        cache_size = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, (size_t)cache_size * sizeof(seq_t));
        if (!new_cache) return (seq_t)0; /* allocation failure – unlikely */
        cache = new_cache;
    }

    /* Initialise the first value a(0)=1 the first time the function runs */
    if (max_computed == 0 && cache[0] == (seq_t)0) {
        cache[0] = (seq_t)1;      /* a(0) */
        max_computed = 0;         /* now we have value for index 0 */
    }

    /* Compute values up to n, using the two different definitions */
    for (idx_t k = max_computed + 1; k <= n; k++) {

        if (k == 1) {
            cache[1] = (seq_t)6;                 /* a(1) = 6 */
        } else if (k < 38) {
            /* a(k) = 6 * 5^(k-1)  ->  a(k) = 5 * a(k-1) */
            cache[k] = (seq_t)5 * cache[k - 1];
        } else {
            /* Compute the recurrence for n >= 38 */
            seq_t sum = (seq_t)0;
            for (idx_t j = 1; j <= 37; j++) {
                sum += cache[k - j];
            }
            cache[k] = (seq_t)4 * sum - (seq_t)10 * cache[k - 38];
        }

        max_computed = k;
    }

    return cache[n];
}
