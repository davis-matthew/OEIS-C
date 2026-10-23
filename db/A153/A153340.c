/*
 * A153340  Number of zig-zag paths from top to bottom of a rectangle of width
 *          8 with n rows.
 * https://oeis.org/A153340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A153340 – number of zig‑zag words of length n over an 8‑letter alphabet.
    Recurrence:
        a(1) =  8,  a(2) = 14,  a(3) = 26,  a(4) = 48
        a(n) = a(n-1) + 3*a(n-2) – 2*a(n-3) – a(n-4)   for n ≥ 5
    This implementation caches already‑computed values so that
    subsequent calls in any order are O(1) after the first evaluation.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)                /* undefined offset */
        return (seq_t)0;

    static seq_t *cache = NULL;  /* 1‑based cache, cache[0] unused */
    static idx_t cache_size = 0;

    /* Return cached value if already computed */
    if (n <= cache_size)
        return cache[n];

    /* Grow cache to hold indices up to n */
    idx_t new_size = n;               /* largest index needed */
    if (cache == NULL) {              /* first allocation */
        cache = (seq_t *)calloc(new_size + 1, sizeof(seq_t));
    } else if (new_size > cache_size) {  /* enlarge */
        cache = (seq_t *)realloc(cache, (new_size + 1) * sizeof(seq_t));
    }
    if (cache == NULL)                /* allocation failed */
        return (seq_t)0;

    /* Initialize the first four terms if necessary */
    static const seq_t init[5] = { 0, 8, 14, 26, 48 };
    for (idx_t i = 1; i <= 4 && i <= new_size; ++i) {
        if (i > cache_size || cache[i] == (seq_t)0)
            cache[i] = init[i];
    }

    /* Compute missing terms using the recurrence */
    idx_t start = cache_size + 1;      /* next index to compute       */
    if (start < 5)
        start = 5;                    /* recurrence starts at n=5    */

    for (idx_t i = start; i <= new_size; ++i) {
        seq_t t1 = cache[i - 1];
        seq_t t2 = cache[i - 2];
        seq_t t3 = cache[i - 3];
        seq_t t4 = cache[i - 4];
        seq_t sum = t1 + (seq_t)3 * t2;
        seq_t sub = (seq_t)2 * t3 + t4;
        cache[i] = sum - sub;
    }

    cache_size = new_size;            /* update largest known index */
    return cache[n];
}
