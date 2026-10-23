/*
 * A192309  0-sequence of reduction of (3n-1) by x^2 -> x+1.
 * https://oeis.org/A192309
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 16, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A192309 – 0‑sequence of reduction of (3n‑1) by x^2 → x+1
 * The values satisfy the linear recurrence
 *   a(n) = 3*a(n-1) – a(n-2) – 3*a(n-3) + a(n-4) + a(n-5)
 * for n ≥ 6 with initial values
 *   a(1)=2, a(2)=2, a(3)=10, a(4)=21, a(5)=49
 * (OEIS offset is 1).
 */

#include <stdlib.h>   /* malloc, free */

seq_t a(idx_t n)
{
    /* 0 is not part of the sequence – return 0 in that case */
    if (n == 0)
        return (seq_t)0;

    /* static cache of computed terms (1‑based index) */
    static seq_t *cache    = NULL;
    static idx_t  size     = 0;

    /* initialise cache with the first five terms */
    if (size == 0) {
        cache = (seq_t *)malloc(5 * sizeof(seq_t));
        if (!cache) return (seq_t)0;           /* allocation failure – unlikely */

        cache[0] = (seq_t)2;   /* a(1) */
        cache[1] = (seq_t)2;   /* a(2) */
        cache[2] = (seq_t)10;  /* a(3) */
        cache[3] = (seq_t)21;  /* a(4) */
        cache[4] = (seq_t)49;  /* a(5) */
        size = 5;
    }

    /* return if already known */
    if (n <= size)
        return cache[n - 1];

    /* extend cache up to n */
    seq_t *new_cache = (seq_t *)malloc(n * sizeof(seq_t));
    if (!new_cache) return (seq_t)0;   /* allocation failure – unlikely */

    /* copy existing terms */
    for (idx_t i = 0; i < size; ++i)
        new_cache[i] = cache[i];

    /* compute new terms using the recurrence */
    for (idx_t i = size; i < n; ++i) {
        new_cache[i] = (seq_t)(3 * new_cache[i - 1]
                               - new_cache[i - 2]
                               - 3 * new_cache[i - 3]
                               + new_cache[i - 4]
                               + new_cache[i - 5]);
    }

    /* replace old cache */
    free(cache);
    cache = new_cache;
    size = n;

    return cache[n - 1];
}
