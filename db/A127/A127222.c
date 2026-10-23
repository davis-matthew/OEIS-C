/*
 * A127222  a(n) = 3^n*pentanacci(n) or (3^n)*A023424(n-1).
 * https://oeis.org/A127222
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 24, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula:  a(n) = 3*a(n-1)+9*a(n-2)+27*a(n-3)+81*a(n-4)+243*a(n-5) 
 * with initial values
 * a(1)=3 , a(2)=27 , a(3)=189 , a(4)=1215 , a(5)=7533
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* static cache to store computed terms for all calls */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;      /* number of terms currently stored */
    static size_t cache_cap = 0;      /* allocated capacity */

    if (n < 1) return 0;  /* offset starts at 1, but n<1 is not expected */

    /* if value already computed, return it */
    if ((size_t)n <= cache_len)
        return cache[(size_t)n - 1];

    /* grow cache to at least n terms */
    if ((size_t)n > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 16;
        while (new_cap < (size_t)n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) { /* allocation failure – abort gracefully */
            free(cache);
            cache = NULL;
            cache_len = cache_cap = 0;
            return 0;
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* initialise the first five terms if not already done */
    if (cache_len < 1) { cache[0] = (seq_t)3;    cache_len = 1; }
    if (cache_len < 2) { cache[1] = (seq_t)27;   cache_len = 2; }
    if (cache_len < 3) { cache[2] = (seq_t)189;  cache_len = 3; }
    if (cache_len < 4) { cache[3] = (seq_t)1215; cache_len = 4; }
    if (cache_len < 5) { cache[4] = (seq_t)7533; cache_len = 5; }

    /* extend cache up to n using the recurrence */
    for (idx_t i = (idx_t)cache_len; i < n; ++i) {
        /* guarantee the indices exist since i >= 5 */
        seq_t term = cache[i-1] * (seq_t)3
                   + cache[i-2] * (seq_t)9
                   + cache[i-3] * (seq_t)27
                   + cache[i-4] * (seq_t)81
                   + cache[i-5] * (seq_t)243;
        cache[i] = term;
    }
    cache_len = (size_t)n;
    return cache[(size_t)n - 1];
}
