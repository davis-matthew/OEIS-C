/*
 * A202526  Number of (n+2) X 4 binary arrays avoiding patterns 000 and 101 in
 *          rows, columns and nw-to-se diagonals.
 * https://oeis.org/A202526
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1) + 2*a(n-3) - 2*a(n-4) - 2*a(n-5) + a(n-7) for
 *            n>9 with initial values: a(1)=198, a(2)=518, a(3)=1084,
 *            a(4)=2232, a(5)=4901, a(6)=10584, a(7)=22525, a(8)=48429,
 *            a(9)=104282
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 22, u64 n <= 51, u128 n <= 109,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* largest n computed and stored */
    static idx_t cache_alloc = 0;  /* allocated size (in seq_t elements) */

    if (n == 0)
        return (seq_t)0;  /* out of range; behaviour unspecified */

    /* initialise cache the first time */
    if (!cache)
    {
        cache_alloc = 10;                      /* will hold indices 0..9 */
        cache = (seq_t *)malloc(cache_alloc * sizeof(seq_t));
        if (!cache)
            return (seq_t)0;                    /* allocation failure */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)198;
        cache[2] = (seq_t)518;
        cache[3] = (seq_t)1084;
        cache[4] = (seq_t)2232;
        cache[5] = (seq_t)4901;
        cache[6] = (seq_t)10584;
        cache[7] = (seq_t)22525;
        cache[8] = (seq_t)48429;
        cache[9] = (seq_t)104282;
        cache_size = 9;
    }

    /* extend allocation if necessary */
    if (n + 1 > cache_alloc)
    {
        idx_t new_alloc = cache_alloc * 2;
        if (new_alloc < n + 1)
            new_alloc = n + 1;
        seq_t *new_cache = (seq_t *)realloc(cache, new_alloc * sizeof(seq_t));
        if (!new_cache)
            return (seq_t)0;  /* allocation failure */
        cache = new_cache;
        cache_alloc = new_alloc;
    }

    /* return cached value if already computed */
    if (n <= cache_size)
        return cache[n];

    /* compute missing terms up to n using the recurrence */
    for (idx_t i = cache_size + 1; i <= n; ++i)
    {
        /* recurrence valid only for i >= 10; for i <= 9 the value is already stored */
        if (i <= 9)
            continue;

        seq_t term = cache[i - 1];
        term += cache[i - 1];   /* 2*a(i-1) */
        term += cache[i - 3];
        term += cache[i - 3];   /* 2*a(i-3) */
        term -= cache[i - 4];
        term -= cache[i - 4];   /* 2*a(i-4) */
        term -= cache[i - 5];
        term -= cache[i - 5];   /* 2*a(i-5) */
        term += cache[i - 7];
        cache[i] = term;
    }

    cache_size = n;
    return cache[n];
}
