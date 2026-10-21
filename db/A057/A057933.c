/*
 * A057933  Floor[(80/81)*10^n].
 * https://oeis.org/A057933
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((80/81) * 10^n) Recurrence: a(1) = 9; for n >= 2
 *            a(n) = 10 * a(n-1) - (n % 9) + 10 * (n % 9 != 0)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values, indices aligned with a(n) */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* first free index (next to write) */
    static size_t cache_cap = 0;      /* allocated capacity */

    /* initialise cache with a[0] = 0 (unused) and a[1] = 9 */
    if (!cache)
    {
        cache_cap = 2;
        cache_size = 2;   /* indices 0 and 1 are valid after init */
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache[0] = 0;     /* dummy, not used */
        cache[1] = 9;
    }

    /* extend cache up to requested index n */
    while (cache_size <= n)
    {
        idx_t i = cache_size;                /* current index to compute */
        seq_t prev = cache[i - 1];
        seq_t term = prev * 10;              /* 10 * a(i-1) */
        idx_t mod9 = i % 9;
        if (mod9 != 0)                       /* add 10 and subtract i%9 */
        {
            term -= (seq_t)mod9;
            term += 10;
        }

        /* ensure there is space for the new element */
        if (cache_size == cache_cap)
        {
            size_t new_cap = cache_cap * 2;
            if (new_cap == 0) new_cap = 2;
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cache_cap = new_cap;
        }

        cache[cache_size] = term;
        cache_size++;
    }

    return cache[n];
}
