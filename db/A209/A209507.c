/*
 * A209507  Half the number of (n+1) X 5 0..2 arrays with every 2 X 2 subblock
 *          having two or four distinct clockwise edge differences.
 * https://oeis.org/A209507
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16*a(n-1) - 65*a(n-2) + 92*a(n-3) - 48*a(n-4) + 8*a(n-5)
 *            Initial values: a(1) = 243 a(2) = 2559 a(3) = 27225 a(4) =
 *            290493 a(5) = 3102219
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* realloc, NULL */

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* 1‑based cache: cache[0] unused */
    static idx_t cache_alloc = 0;     /* allocated size (max index that can be stored) */
    static idx_t cache_computed = 0;  /* highest n for which cache[n] is valid */

    if (n == 0) return 0;  /* undefined for the offset, but return 0 safely */

    /* Ensure we have space for all indices up to n. */
    if ((idx_t)(n + 1) > cache_alloc) {
        idx_t new_size = n + 16;      /* spare capacity to reduce reallocs */
        if (new_size < 32) new_size = 32;  /* minimal allocation */
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failure – return 0 */
        cache = tmp;
        cache_alloc = new_size;
    }

    /* Initialise the first five values once. */
    if (cache_computed < 5) {
        cache[1] = (seq_t)243;
        cache[2] = (seq_t)2559;
        cache[3] = (seq_t)27225;
        cache[4] = (seq_t)290493;
        cache[5] = (seq_t)3102219;
        cache_computed = 5;
    }

    /* Compute any missing terms up to n. */
    for (idx_t i = cache_computed + 1; i <= n; ++i) {
        /* Calculate the recurrence using only positive intermediate sums
           to avoid underflow on unsigned integer types. */
        seq_t pos = (seq_t)16 * cache[i-1] + (seq_t)92 * cache[i-3] + (seq_t)8 * cache[i-5];
        seq_t neg = (seq_t)65 * cache[i-2] + (seq_t)48 * cache[i-4];
        cache[i] = pos - neg;
    }
    cache_computed = n;

    return cache[n];
}
