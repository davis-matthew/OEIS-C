/*
 * A233474  Numbers m such that 5*T(m)-1 is a square, where T = A000217.
 * https://oeis.org/A233474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + 38*a(n-2) - 38*a(n-3) - a(n-4) + a(n-5) with
 *            a(1)=1, a(2)=4, a(3)=52, a(4)=169, a(5)=1993
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 13, u64 n <= 25, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0; /* number of terms stored */
    static idx_t cache_cap = 0; /* allocated capacity */

    if (n == 0) return (seq_t)0; /* offset is 1, values for n<1 are unused */

    /* initialise base terms on first call */
    if (cache_len == 0) {
        cache_cap = 5;
        cache     = (seq_t *)malloc((size_t)cache_cap * sizeof(seq_t));
        cache[0]  = (seq_t)1;
        cache[1]  = (seq_t)4;
        cache[2]  = (seq_t)52;
        cache[3]  = (seq_t)169;
        cache[4]  = (seq_t)1993;
        cache_len = 5;
    }

    /* ensure capacity */
    if (n > (idx_t)cache_cap) {
        idx_t new_cap = cache_cap * 2;
        if (new_cap < n) new_cap = n;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute missing terms */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        if (i <= 5) continue; /* base terms already set */
        /* recurrence: a(i) = a(i-1) + 38*a(i-2) - 38*a(i-3) - a(i-4) + a(i-5) */
        seq_t val = cache[i-2]
                  + (seq_t)(38) * cache[i-3]
                  - (seq_t)(38) * cache[i-4]
                  -            cache[i-5]
                  +            cache[i-6];
        cache[i-1] = val;
    }
    cache_len = n; /* we now have computed up to n */

    return cache[n-1];
}
