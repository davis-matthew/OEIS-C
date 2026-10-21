/*
 * A051618  a(n) = (4*n+6)(!^4)/6(!^4).
 * https://oeis.org/A051618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*(2*n+3)*a(n-1); a(0)=1
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache  = NULL;   /* array of previously computed terms   */
    static idx_t  size   = 0;      /* number of elements currently stored  */
    static idx_t  cap    = 0;      /* allocated capacity of cache          */

    /* Ensure the cache can hold up to n (inclusive) */
    if ((idx_t)n >= size) {
        /* Grow capacity if needed */
        idx_t new_cap = cap ? cap : 16;
        while (new_cap <= n) new_cap <<= 1;
        if (new_cap > cap) {
            /* realloc returns void* in C++; cast explicitly */
            cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
            cap   = new_cap;
        }

        /* Initialise first term if cache is empty */
        if (size == 0) {
            cache[0] = (seq_t)1;   /* a(0) = 1 */
            size     = 1;
        }

        /* Compute missing terms up to n using recurrence a(n)=2*(2*n+3)*a(n-1) */
        for (idx_t i = size; i <= n; ++i) {
            seq_t mul = ((seq_t)2) * (((seq_t)2 * (seq_t)i) + (seq_t)3);
            cache[i] = cache[i-1] * mul;
        }
        size = n + 1;
    }

    return cache[n];
}
