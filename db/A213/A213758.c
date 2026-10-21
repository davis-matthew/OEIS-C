/*
 * A213758  Antidiagonal sums of the convolution array A213756.
 * https://oeis.org/A213758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 27, u64 n <= 59, u128 n <= 100,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 6*a(n-1) - 14*a(n-2) + 16*a(n-3) - 9*a(n-4) + 2*a(n-5)
   initial values: a(1)=1, a(2)=9, a(3)=40, a(4)=130, a(5)=355
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle offset: sequence starts at n=1.  For n<1 return 0. */
    if (n < 1)
        return (seq_t)0;

    /* Static cache to store computed values.  cache[0] holds a(1). */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_capacity = 0;

    /* First call: allocate initial capacity for the five known terms. */
    if (cache == NULL) {
        cache_capacity = 8;           /* start a bit larger than 5 */
        cache = (seq_t *)malloc(cache_capacity * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0; /* out of memory */

        /* initialise first 5 known terms */
        cache[0] = (seq_t)1;   /* a(1) */
        cache[1] = (seq_t)9;   /* a(2) */
        cache[2] = (seq_t)40;  /* a(3) */
        cache[3] = (seq_t)130; /* a(4) */
        cache[4] = (seq_t)355; /* a(5) */

        cache_size = 5;
    }

    /* If requested term is already known, return it directly. */
    if ((size_t)n <= cache_size)
        return cache[n - 1];

    /* Ensure cache large enough for new terms. */
    size_t needed = (size_t)n;
    if (needed > cache_capacity) {
        size_t new_capacity = cache_capacity;
        while (new_capacity < needed)
            new_capacity *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_capacity * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* out of memory */
        cache = tmp;
        cache_capacity = new_capacity;
    }

    /* Compute missing terms up to n using the recurrence. */
    for (size_t i = cache_size + 1; i <= needed; ++i) {
        /* Indices in cache array (0‑based):
           a(i-1)  -> cache[i-2]
           a(i-2)  -> cache[i-3]
           a(i-3)  -> cache[i-4]
           a(i-4)  -> cache[i-5]
           a(i-5)  -> cache[i-6]   */
        seq_t sum1 = (seq_t)6 * cache[i - 2];
        sum1 += (seq_t)16 * cache[i - 4];
        sum1 += (seq_t)2 * cache[i - 6];

        seq_t sum2 = (seq_t)14 * cache[i - 3];
        sum2 += (seq_t)9 * cache[i - 5];

        cache[i - 1] = sum1 - sum2;
    }

    cache_size = needed;
    return cache[n - 1];
}
