/*
 * A157746  Expansion of 1/(x^11 + x^10 + x^6 + x^5 + x^4 + x^2 + 1).
 * https://oeis.org/A157746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) + a(n-2) + a(n-4) + a(n-5) + a(n-6) + a(n-10) + a(n-11) = 0
 *            with initial values a(0)=1, a(1)=0, a(2)=-1, a(3)=0, a(4)=0,
 *            a(5)=-1, a(6)=0, a(7)=2, a(8)=1, a(9)=-1, a(10)=-1 The
 *            recurrence is applied for n ≥ 11
 * Range:     i8 n <= 39, i16 n <= 63, i32 n <= 63, i64 n <= 63, i128 n <= 63,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = (seq_t *)NULL;
static idx_t cache_size = 0;

seq_t a(idx_t n)
{
    /* First time: allocate and initialise the base terms */
    if (cache_size == 0) {
        cache_size = 11;                /* positions 0 … 10 */
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) return 0;          /* allocation unlikely to fail */

        cache[0]  =  1;
        cache[1]  =  0;
        cache[2]  = -1;
        cache[3]  =  0;
        cache[4]  =  0;
        cache[5]  = -1;
        cache[6]  =  0;
        cache[7]  =  2;
        cache[8]  =  1;
        cache[9]  = -1;
        cache[10] = -1;
    }

    /* Return already computed term */
    if (n < cache_size)
        return cache[n];

    /* Grow the cache to accommodate index n */
    idx_t new_size = n + 1;
    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) return 0;          /* reallocation failure unlikely */
    cache = tmp;

    /* Compute missing terms using the recurrence */
    for (idx_t i = cache_size; i <= n; ++i) {
        seq_t sum = 0;
        sum += cache[i - 2];
        sum += cache[i - 4];
        sum += cache[i - 5];
        sum += cache[i - 6];
        sum += cache[i -10];
        sum += cache[i -11];
        cache[i] = -sum;
    }

    cache_size = new_size;
    return cache[n];
}
