/*
 * A152718  a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) - a(n-4) + a(n-5), n > 5.
 * https://oeis.org/A152718
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) - a(n-2) + 2*a(n-3) - a(n-4) + a(n-5), a(0)=1,
 *            a(1)=1, a(2)=1, a(3)=1, a(4)=2, a(5)=4
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 36, u64 n <= 69, u128 n <= 135,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>


seq_t a(idx_t n)
{
    /* static cache of computed terms */
    static seq_t *cache = NULL;      /* allocated array */
    static size_t cap   = 0;         /* allocated capacity */
    static size_t used  = 0;         /* number of initialized terms */

    /* initialise cache with the first six terms */
    if (!cache) {
        used = 6;
        cap  = 6;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0; /* allocation failed */
        cache[0] = 1;
        cache[1] = 1;
        cache[2] = 1;
        cache[3] = 1;
        cache[4] = 2;
        cache[5] = 4;
    }

    /* If the value is already known, return it */
    if (n < (idx_t)used) return cache[n];

    /* Ensure the array is large enough */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? cap : 1;
        while (new_cap <= (size_t)n + 1) new_cap <<= 1;
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) {
            free(cache);
            cache = NULL;
            cap   = 0;
            used  = 0;
            return (seq_t)0; /* allocation failed */
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing terms up to n */
    for (size_t i = used; i <= (size_t)n; ++i) {
        seq_t t0 = cache[i-1];
        seq_t t1 = cache[i-2];
        seq_t t2 = cache[i-3];
        seq_t t3 = cache[i-4];
        seq_t t4 = cache[i-5];
        cache[i] = 2*t0 - t1 + 2*t2 - t3 + t4;
    }
    used = (size_t)n + 1;

    return cache[n];
}
