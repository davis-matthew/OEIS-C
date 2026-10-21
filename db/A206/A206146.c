/*
 * A206146  Number of (n+1) X 5 0..2 arrays with every 2 X 3 or 3 X 2 subblock
 *          having exactly three counterclockwise and three clockwise edge
 *          increases.
 * https://oeis.org/A206146
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 3, u32 n <= 7, u64 n <= 17, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 16*a(n-1) - 65*a(n-2) + 92*a(n-3) - 48*a(n-4) + 8*a(n-5)
   Initial values: a(1)=486, a(2)=5118, a(3)=54450, a(4)=580986, a(5)=6204438
   (offset 1). */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* offset is 1; undefined for 0 */

    static seq_t *cache = NULL;        /* cached sequence values */
    static idx_t cache_size = 0;       /* number of values stored (>=1) */

    /* Initial allocation and seeding */
    if (cache == NULL) {
        cache_size = (n > 5 ? n : 5);
        cache = (seq_t *)calloc((size_t)cache_size, sizeof(seq_t));
        if (!cache) exit(1);           /* memory error */

        cache[0] = (seq_t)486;         /* a(1) */
        if (cache_size >= 2) cache[1] = (seq_t)5118;     /* a(2) */
        if (cache_size >= 3) cache[2] = (seq_t)54450;    /* a(3) */
        if (cache_size >= 4) cache[3] = (seq_t)580986;   /* a(4) */
        if (cache_size >= 5) cache[4] = (seq_t)6204438;  /* a(5) */

        /* compute recurrence up to cache_size if needed */
        for (idx_t i = 6; i <= cache_size; ++i) {
            cache[i-1] =
                16 * cache[i-2]        -   /* a(n-1) */
                65 * cache[i-3]        +   /* a(n-2) */
                92 * cache[i-4]        -   /* a(n-3) */
                48 * cache[i-5]        +   /* a(n-4) */
                8  * cache[i-6];            /* a(n-5) */
        }
    }
    /* Grow cache if needed and compute new values */
    else if (n > cache_size) {
        idx_t old_size = cache_size;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)n * sizeof(seq_t));
        if (!tmp) exit(1);
        cache = tmp;
        /* Compute terms from old_size+1 to n */
        for (idx_t i = old_size + 1; i <= n; ++i) {
            cache[i-1] =
                16 * cache[i-2]        -   /* a(n-1) */
                65 * cache[i-3]        +   /* a(n-2) */
                92 * cache[i-4]        -   /* a(n-3) */
                48 * cache[i-5]        +   /* a(n-4) */
                8  * cache[i-6];            /* a(n-5) */
        }
        cache_size = n;
    }

    return cache[n-1];
}
