/*
 * A025859  Expansion of 1/((1-x^4)*(1-x^5)*(1-x^7)).
 * https://oeis.org/A025859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = coefficient of x^n in 1/((1−x^4)(1−x^5)(1−x^7)).  */
/* From (1−x^4)(1−x^5)(1−x^7) = 1−x^4−x^5−x^7+x^9+x^11+x^12−x^16
   we obtain the recurrence
          a(n) = a(n−4)+a(n−5)+a(n−7)
                 −a(n−9)−a(n−11)−a(n−12)+a(n−16)
     for all n≥0, with a(k)=0 for k<0.  */
#include <stdlib.h>

static seq_t *cache = NULL;   /* DP table: cache[i] = a(i) */
static idx_t cached_n = 0;    /* largest n that has been computed */
static idx_t cache_size = 0;  /* current allocated size of cache */

/* Ensure that cache has room for indices 0 … need (inclusive). */
static void resize_cache(idx_t need)
{
    if (need + 1 <= cache_size) return;

    idx_t old_size = cache_size;
    idx_t new_size = cache_size ? cache_size * 2 : 16;
    if (new_size < need + 1) new_size = need + 1;

    seq_t *tmp = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!tmp) abort();                /* memory exhausted */

    cache = tmp;
    cache_size = new_size;

    /* Zero newly allocated entries. */
    for (idx_t i = old_size; i < new_size; ++i)
        cache[i] = 0;
}

seq_t a(idx_t n)
{
    /* First call: allocate cache and initialise base case. */
    if (!cache) {
        resize_cache(n);
        cache[0] = 1;
        cached_n = 0;
    }

    if (n > cached_n) {
        resize_cache(n);
        /* Compute values up to n. */
        for (idx_t i = cached_n + 1; i <= n; ++i) {
            seq_t pos = 0, neg = 0;

            if (i >= 4) pos += cache[i - 4];
            if (i >= 5) pos += cache[i - 5];
            if (i >= 7) pos += cache[i - 7];
            if (i >= 16) pos += cache[i - 16];

            if (i >= 9) neg += cache[i - 9];
            if (i >= 11) neg += cache[i - 11];
            if (i >= 12) neg += cache[i - 12];

            cache[i] = pos - neg;   /* pos >= neg always for this recurrence */
        }
        cached_n = n;
    }

    return cache[n];
}
