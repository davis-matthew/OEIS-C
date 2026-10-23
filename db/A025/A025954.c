/*
 * A025954  Expansion of 1/((1-2*x)*(1-3*x)*(1-9*x)*(1-12*x)).
 * https://oeis.org/A025954
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 17, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   Sequence A025954: Expansion of 1/((1-2*x)*(1-3*x)*(1-9*x)*(1-12*x))
   Recurrence: a(0)=1, a(1)=26, a(2)=457, a(3)=6854, 
               a(n)=26*a(n-1)-219*a(n-2)+666*a(n-3)-648*a(n-4)  (n≥4)
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;     /* dynamic array of cached terms */
    static size_t cached = 0;       /* number of terms currently stored */
    static size_t cap = 0;          /* allocated capacity of cache */

    /* Return cached value if already computed */
    if (n < cached)
        return cache[(size_t)n];

    /* Ensure enough space for up to n */
    size_t needed = (size_t)n + 1;
    if (needed > cap) {
        size_t newcap = cap ? cap * 2 : 16;
        if (newcap < needed)
            newcap = needed;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp)
            return (seq_t)0;       /* allocation failure → return 0 */

        /* For C++ builds, default‑construct the newly allocated elements */
#ifndef __cplusplus
        /* nothing needed – seq_t is a plain integer type in C */
#else
        for (size_t i = cap; i < newcap; ++i)
            new(&tmp[i]) seq_t();  /* placement new */
#endif

        cache = tmp;
        cap = newcap;
    }

    /* Compute missing terms using the recurrence */
    for (size_t i = cached; i < needed; ++i) {
        if (i == 0)
            cache[i] = (seq_t)1;
        else if (i == 1)
            cache[i] = (seq_t)26;
        else if (i == 2)
            cache[i] = (seq_t)457;
        else if (i == 3)
            cache[i] = (seq_t)6854;
        else
            cache[i] = (seq_t)26 * cache[i - 1]
                     - (seq_t)219 * cache[i - 2]
                     + (seq_t)666 * cache[i - 3]
                     - (seq_t)648 * cache[i - 4];
    }

    cached = needed;
    return cache[(size_t)n];
}
