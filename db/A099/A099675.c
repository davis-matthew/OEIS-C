/*
 * A099675  Partial sums of repdigits of A002282.
 * https://oeis.org/A099675
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   A099675: a(1)=8, a(2)=96, a(3)=984; a(n) = 12*a(n-1) - 21*a(n-2)
 *            + 10*a(n-3) for n>=4
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache: cache[i] holds a(i), i>=1. */
    static seq_t *cache = NULL;          /* array of computed values */
    static idx_t cache_len = 0;          /* highest index stored (cache[1..cache_len]) */

    if (n == 0) return 0;    /* offset is 1; n=0 is unused but safe */

    if (n <= cache_len)      /* already computed */
        return cache[n];

    /* Grow cache to hold values up to n */
    size_t new_cap = (size_t)(n + 1);      /* need indices 0..n */
    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (tmp == NULL && new_cap > 0)        /* realloc failed; abort */
        return 0;
    cache = tmp;

    /* Compute missing terms */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i == 1)
            cache[i] = (seq_t)8;
        else if (i == 2)
            cache[i] = (seq_t)96;
        else if (i == 3)
            cache[i] = (seq_t)984;
        else {
            seq_t term1 = (seq_t)12 * cache[i-1];
            seq_t term2 = (seq_t)21 * cache[i-2];
            seq_t term3 = (seq_t)10 * cache[i-3];
            cache[i] = term1 - term2 + term3;
        }
    }
    cache_len = n;
    return cache[n];
}
