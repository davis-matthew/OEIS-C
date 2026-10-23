/*
 * A141365  Expansion of (1-5x^2-7x^3-2x^4+x^6)/((1-x)(1-x^3)^2).
 * https://oeis.org/A141365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = a(n-1)+2*a(n-3)-2*a(n-4)-a(n-6)+a(n-7) * Initial values:
 *            a(0)=1, a(1)=1, a(2)=-4, a(3)=-9, a(4)=-11, a(5)=-21, a(6)=-31
 * Range:     i8 n <= 13, i16 n <= 42, i32 n <= 42, i64 n <= 42, i128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;   /* number of elements currently stored */
static size_t cache_cap = 0;  /* allocated capacity */

seq_t a(idx_t n)
{
    /* Initialize the cache with the first 7 terms */
    if (cache == NULL)
    {
        const idx_t init_size = 7;              /* indices 0..6 */
        cache = (seq_t *)malloc(init_size * sizeof(seq_t));
        if (cache == NULL)
            exit(1);                            /* memory allocation failed */

        cache[0] = 1;
        cache[1] = 1;
        cache[2] = -4;
        cache[3] = -9;
        cache[4] = -11;
        cache[5] = -21;
        cache[6] = -31;

        cache_len = init_size;
        cache_cap = init_size;
    }

    /* Expand the cache up to index n using the recurrence relation */
    while (cache_len <= n)
    {
        idx_t i = cache_len;                    /* index to compute */

        seq_t val = cache[i - 1]
                  + 2 * cache[i - 3]
                  - 2 * cache[i - 4]
                  - cache[i - 6]
                  + cache[i - 7];

        /* Ensure there is enough space in the cache */
        if (cache_len == cache_cap)
        {
            size_t new_cap = cache_cap * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            if (tmp == NULL)
                exit(1);                        /* memory allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }

        cache[cache_len] = val;
        ++cache_len;
    }

    return cache[n];
}
