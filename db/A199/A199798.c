/*
 * A199798  y-values in the solution to 17*x^2 + 16 = y^2.
 * https://oeis.org/A199798
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 66*a(n-3) - a(n-6) for n ≥ 7, a(1)=4, a(2)=13, a(3)=21,
 *            a(4)=132, a(5)=837, a(6)=1373
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle indices < 1 (offset is 1).  The sequence is undefined
       for n < 1; we return 0 in this case as a harmless default. */
    if (n == 0)
        return (seq_t)0;

    /* Static cache where cache[0] holds a(1), cache[1] holds a(2), …   */
    static seq_t *cache = NULL;
    static size_t cache_capacity = 0;
    static int first_call = 1;

    /* Initialise cache with the first six terms */
    if (first_call) {
        cache_capacity = 6;
        cache = (seq_t*)calloc(cache_capacity, sizeof(seq_t));
        if (!cache) abort(); /* allocation failure */

        cache[0] = (seq_t)4;   /* a(1) */
        cache[1] = (seq_t)13;  /* a(2) */
        cache[2] = (seq_t)21;  /* a(3) */
        cache[3] = (seq_t)132; /* a(4) */
        cache[4] = (seq_t)837; /* a(5) */
        cache[5] = (seq_t)1373;/* a(6) */

        first_call = 0;
    }

    /* Ensure the cache can hold the requested term */
    size_t needed = (size_t)n; /* we use 1‑based index but array is 0‑based */
    if (needed > cache_capacity) {
        size_t new_capacity = needed;
        seq_t *tmp = (seq_t*)realloc(cache, new_capacity * sizeof(seq_t));
        if (!tmp) abort(); /* allocation failure */
        cache = tmp;
        cache_capacity = new_capacity;
    }

    /* Compute missing terms using the recurrence */
    for (size_t i = 6; i < (size_t)n; ++i) {
        /* a(i+1) = 66*a(i-2) - a(i-5)  (0‑based indices) */
        cache[i] = (seq_t)66 * cache[i - 3] - cache[i - 6];
    }

    return cache[(size_t)n - 1];
}
