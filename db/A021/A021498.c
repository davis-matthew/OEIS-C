/*
 * A021498  Decimal expansion of 1/494.
 * https://oeis.org/A021498
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   not stated
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS: A021498 – Decimal expansion of 1/494
    Recurrence:  a(0)=0, a(1)=0, a(2)=2, a(3)=0, a(4)=2, a(5)=4, a(6)=2, a(7)=9,
                 a(8)=1, a(9)=4, a(10)=9,
                 a(n)=a(n-1)-a(n-9)+a(n-10)   for n>10  */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoised values */
    static seq_t *cache = NULL;
    static idx_t  cache_size = 0;
    static idx_t  last_computed = 0;

    /* initial digits for indices 0..10 */
    static const seq_t init_vals[11] = {
        (seq_t)0, (seq_t)0, (seq_t)2, (seq_t)0, (seq_t)2,
        (seq_t)4, (seq_t)2, (seq_t)9, (seq_t)1, (seq_t)4, (seq_t)9
    };

    /* first call – allocate minimal cache and copy the initial values */
    if (cache == NULL) {
        cache_size = 11;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;                /* allocation failure */
        for (idx_t i = 0; i < cache_size; ++i)
            cache[i] = init_vals[i];
        last_computed = 10;
    }

    /* grow the cache if the requested index is beyond the current size */
    if (n >= cache_size) {
        size_t new_cap = cache_size ? cache_size * 2 : 16;
        while (new_cap <= (size_t)n + 1)
            new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0; /* allocation failure */
        cache = tmp;
        cache_size = new_cap;
    }

    /* compute missing terms up to n if necessary */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        int val;
        if (i <= 10) {
            /* values 0..10 already present – guaranteed to be filled */
            val = (int)cache[i];
        } else {
            /* recurrence: a(i)=a(i-1)-a(i-9)+a(i-10) */
            val = (int)cache[i-1] - (int)cache[i-9] + (int)cache[i-10];
        }
        cache[i] = (seq_t)val;
    }
    last_computed = n;

    return cache[n];
}
