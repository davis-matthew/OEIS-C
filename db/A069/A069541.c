/*
 * A069541  Duplicate of A062768.
 * https://oeis.org/A069541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 18, i32 n <= 18, i64 n <= 18, i128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Numbers that are multiples of 6 with digit sum equal to 6. */

static inline seq_t digit_sum(seq_t x)
{
    seq_t sum = 0;
    while (x != (seq_t)0)
    {
        sum += x % (seq_t)10;
        x /= (seq_t)10;
    }
    return sum;
}

seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of terms stored */
    static size_t cache_cap = 0;  /* capacity of the cache array */
    static seq_t candidate = (seq_t)6;  /* next multiple of 6 to test */

    if (n == (idx_t)0)
        return (seq_t)0;  /* undefined for n=0, but return 0 */

    /* Initialize cache on first call */
    if (cache == NULL)
    {
        cache_cap = 8;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        candidate = (seq_t)6;
    }

    /* Grow cache until it contains at least n terms */
    while (cache_len < n)
    {
        seq_t val = candidate;
        candidate += (seq_t)6;

        if (digit_sum(val) == (seq_t)6)
        {
            if ((size_t)cache_len >= cache_cap)
            {
                cache_cap *= 2;
                cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
            }
            cache[cache_len++] = val;
        }
    }

    return cache[n - (idx_t)1];
}
