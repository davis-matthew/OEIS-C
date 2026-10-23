/*
 * A019488  Expansion of 1/((1-4*x)*(1-6*x)*(1-11*x)).
 * https://oeis.org/A019488
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Formula: a(n) = (10*4^(n+1) - 21*6^(n+1) + 11^(n+2)) / 35
 * Recurrence: a(0)=1, a(1)=21, a(2)=307
 *              a(n) = 21*a(n-1) - 134*a(n-2) + 264*a(n-3)  for n > 2
 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;        /* memoised values                     */
    static idx_t cache_size = 0;        /* number of elements allocated       */

    /* initialise cache with first three terms */
    if (cache == NULL) {
        cache_size = 3;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_size);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)21;
        cache[2] = (seq_t)307;
    }

    /* grow cache if necessary */
    if (n >= cache_size) {
        idx_t new_size = cache_size * 2;
        if (new_size < n + 1)
            new_size = n + 1;
        cache = (seq_t *)realloc(cache, sizeof(seq_t) * new_size);
        /* initialise the newly allocated part to 0 */
        for (idx_t i = cache_size; i < new_size; ++i)
            cache[i] = (seq_t)0;
        cache_size = new_size;
    }

    /* already computed? */
    if (n <= 2 || cache[n] != (seq_t)0)
        return cache[n];

    /* compute missing values using the linear recurrence */
    for (idx_t i = 3; i <= n; ++i) {
        if (cache[i] == (seq_t)0) {
            /* 21*a(i-1) - 134*a(i-2) + 264*a(i-3) */
            seq_t t1 = cache[i-1];
            seq_t t2 = cache[i-2];
            seq_t t3 = cache[i-3];
            cache[i] = 21 * t1 - 134 * t2 + 264 * t3;
        }
    }

    return cache[n];
}
