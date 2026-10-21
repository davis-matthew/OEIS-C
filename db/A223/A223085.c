/*
 * A223085  Trajectory of 82 under the map n-> A006369(n).
 * https://oeis.org/A223085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 77, u32 n <= 192, u64 n <= 455,
 *            u128 n <= 1209, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Trajectory of 82 under the map n -> A006369(n)
 *
 * Let f(x) = floor((x+1)/3).
 * Then A006369(x) = 
 *   x + f(x)   if x % 3 != 0 
 *   x - f(x)   if x % 3 == 0
 *
 * The sequence a(n) is defined by a(1) = 82 and a(n+1) = A006369(a(n)).
 */

#include <stdlib.h>
#include <stdint.h>

seq_t next_value(seq_t x)
{
    seq_t k = (x + 1) / 3;          /* floor((x+1)/3) */
    return (x % 3 == 0) ? (x - k) : (x + k);
}

seq_t a(idx_t n)
{
    /* static cache of already computed terms, 1‑based */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* capacity of cache array */
    static size_t cache_len = 0;   /* number of terms stored (>=1) */

    if (n == 0) return 0;          /* not used but safe */

    /* initialisation: allocate a small cache and store a(1)=82 */
    if (cache == NULL)
    {
        cache_cap = 256;           /* initial capacity */
        cache     = (seq_t*)malloc(cache_cap * sizeof(seq_t));
        if (cache == NULL) return 0;  /* malloc failed */
        cache[1]  = (seq_t)82;     /* a(1) */
        cache_len = 1;
    }

    /* grow cache if necessary */
    if ((size_t)n > cache_cap)
    {
        size_t new_cap = cache_cap;
        while ((size_t)n > new_cap)
            new_cap *= 2;          /* double until enough */
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (new_cache == NULL) return 0; /* realloc failed */
        cache     = new_cache;
        cache_cap = new_cap;
    }

    /* compute terms until n is reached */
    while (cache_len < (size_t)n)
    {
        seq_t prev = cache[cache_len];
        cache[cache_len + 1] = next_value(prev);
        ++cache_len;
    }

    return cache[n];
}
