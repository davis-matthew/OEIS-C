/*
 * A052633  E.g.f. x^2*(1+x-x^2)/(1-x)^2.
 * https://oeis.org/A052633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 0 for n = 0,1 * a(2) = 2 * a(3) = 18 * a(n) = a(n-1) *
 *            n^2 / (n-1) for n > 3 * Equivalently for n > 2: a(n) = n * n!
 *            (n! means factorial of n)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    /* Initialize cache to first four terms */
    if (cache == NULL) {
        /* allocate space for indices 0..3 */
        cache = (seq_t *)realloc(NULL, 4 * sizeof(seq_t));
        cache[0] = 0;
        cache[1] = 0;
        cache[2] = 2;
        cache[3] = 18;
        cache_len = 4;
    }

    /* Return cached value if already computed */
    if (n < cache_len)
        return cache[n];

    /* Extend cache to include new terms up to n */
    idx_t new_len = n + 1;                /* size after expansion */
    cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));

    /* Compute missing terms */
    for (idx_t i = cache_len; i <= n; ++i) {
        if (i < 4) {
            /* Base cases already set; just copy for safety */
            cache[i] = cache[i];
        } else {
            /* recurrence: a(i) = a(i-1) * i^2 / (i-1) */
            seq_t prev = cache[i - 1];
            seq_t div  = prev / ((seq_t)(i - 1));          /* exact division */
            seq_t sq   = (seq_t)i * (seq_t)i;             /* i^2   */
            cache[i]   = div * sq;                        /* a(i)  */
        }
    }

    cache_len = new_len;
    return cache[n];
}
