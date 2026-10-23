/*
 * A017862  Expansion of 1/(1-x^7-x^8-x^9-x^10-x^11-x^12-x^13).
 * https://oeis.org/A017862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 38, u16 n <= 65, u32 n <= 120, u64 n <= 229,
 *            u128 n <= 448, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = a(n-7)+a(n-8)+a(n-9)+a(n-10)+a(n-11)+a(n-12)+a(n-13),
   a(0)=1, a(1)=a(2)=…=a(6)=0, a(7)=a(8)=…=a(13)=1 */

#include <stdlib.h>

static seq_t *cache = NULL;   /* buffer holding computed values */
static idx_t  cache_size = 0; /* number of valid entries in cache */

static seq_t *grow_cache(idx_t new_size)
{
#if defined(__cplusplus)
    /* In C++ use new[]/delete[] to create a properly constructed array */
    seq_t *new_cache = new seq_t[new_size];
    for (idx_t i = 0; i < cache_size && i < new_size; ++i)
        new_cache[i] = cache[i];
    delete[] cache;
    return new_cache;
#else
    /* In C use realloc which yields raw storage (OK because seq_t is an
       arithmetic or POD type in the C builds) */
    return (seq_t *)realloc(cache, new_size * sizeof(seq_t));
#endif
}

seq_t a(idx_t n)
{
    /* ensure cache is large enough */
    if (cache_size <= n) {
        cache = grow_cache(n + 1);
        if (cache == NULL)
            exit(1);                       /* allocation failed */

        /* compute values from cache_size up to n */
        for (idx_t i = cache_size; i <= n; ++i) {
            seq_t val;
            if (i == 0)
                val = (seq_t)1;
            else if (i <= 6)
                val = (seq_t)0;
            else if (i <= 13)
                val = (seq_t)1;
            else {
                seq_t sum = 0;
                sum += cache[i -  7];
                sum += cache[i -  8];
                sum += cache[i -  9];
                sum += cache[i - 10];
                sum += cache[i - 11];
                sum += cache[i - 12];
                sum += cache[i - 13];
                val = sum;
            }
            cache[i] = val;
        }
        cache_size = n + 1;
    }
    return cache[n];
}
