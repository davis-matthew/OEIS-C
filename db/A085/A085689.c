/*
 * A085689  a(1) = 4; a(n) = if n == 2 mod 3 then a(n-1)/2, if n == 0 mod 3
 *          then a(n-1)*2, if n == 1 mod 3 then a(n-1)*3.
 * https://oeis.org/A085689
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 27, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=4; for n>1,
   if n%3==2   a(n)=a(n-1)/2
   if n%3==0   a(n)=a(n-1)*2
   if n%3==1   a(n)=a(n-1)*3 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;               /* undefined for n<1, return 0 */

    /* static cache for memoization */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_max = 0;

    /* initial allocation */
    if (!cache) {
        cache_cap = 4;                  /* room for indices 0..3 */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)4;            /* a(1)=4 */
        cache_max = 1;
    }

    /* grow cache if necessary */
    if (n >= cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap <= n)
            new_cap += new_cap;       /* double until enough */
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }

    /* compute missing values up to n */
    for (idx_t i = cache_max + 1; i <= n; ++i) {
        idx_t r = i % (idx_t)3;
        if (r == 2)
            cache[i] = cache[i - 1] / (seq_t)2;
        else if (r == 0)
            cache[i] = cache[i - 1] * (seq_t)2;
        else /* r == 1 */
            cache[i] = cache[i - 1] * (seq_t)3;
    }
    cache_max = n;

    return cache[n];
}
