/*
 * A121803  a(n)= 7*a(n-1) -4*a(n-2) -45*a(n-3) +64*a(n-4) +55*a(n-5)
 *          -128*a(n-6) +52*a(n-7).
 * https://oeis.org/A121803
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 7*a(n-1) -4*a(n-2) -45*a(n-3) +64*a(n-4) +55*a(n-5)
 *            -128*a(n-6) +52*a(n-7) Initial values: a(1)=0, a(2)=28,
 *            a(3)=432, a(4)=2083, a(5)=11262, a(6)=54976, a(7)=274107
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* static cache: 1‑based indices, cache[0] unused */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;      /* number of allocated slots */
    static idx_t computed = 0;        /* highest index computed so far */
    static bool initialized = false;

    /* base values (offset 1) */
    const seq_t base[8] = {0, 0, 28, 432, 2083, 11262, 54976, 274107};

    /* initialise on first call */
    if (!initialized)
    {
        /* allocate at least 8 slots (1‑based) */
        cache_size = 16;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (!cache) abort();                /* allocation failed */
        /* set unused index 0 to zero */
        cache[0] = 0;
        for (idx_t i = 1; i <= 7; ++i)
            cache[i] = base[i];
        computed = 7;
        initialized = true;
    }

    /* extend cache if needed */
    while (cache_size <= n)
    {
        idx_t new_size = cache_size * 2;
        if (new_size <= n) new_size = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cache_size = new_size;
    }

    /* return directly if already computed */
    if (n <= computed)
        return cache[n];

    /* compute remaining terms */
    for (idx_t i = computed + 1; i <= n; ++i)
    {
        seq_t term = 0;

        /* positive contributions */
        term += cache[i-1] * (seq_t)7;
        term += cache[i-4] * (seq_t)64;
        term += cache[i-5] * (seq_t)55;
        term += cache[i-7] * (seq_t)52;

        /* negative contributions */
        term -= cache[i-2] * (seq_t)4;
        term -= cache[i-3] * (seq_t)45;
        term -= cache[i-6] * (seq_t)128;

        cache[i] = term;
    }

    computed = n;
    return cache[n];
}
