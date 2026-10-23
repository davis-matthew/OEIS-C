/*
 * A135365  a(n) = 3*a(n-1) - a(n-3) + 3*a(n-4), with initial values 1,3,9,11.
 * https://oeis.org/A135365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1) - a(n-3) + 3*a(n-4), with a(0)=1, a(1)=3,
 *            a(2)=9, a(3)=11
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static mutable state for memoisation   */
    static idx_t last_computed = (idx_t)-1;   /* largest n that has been computed   */
    static size_t capacity   = 0;            /* number of slots allocated          */
    static seq_t *cache      = NULL;         /* array holding cached values        */

    /* initialise cache with the first 4 terms */
    if (cache == NULL) {
        capacity   = 4;
        cache      = (seq_t*)malloc(capacity * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)9;
        cache[3] = (seq_t)11;
        last_computed = 3;
    }

    /* already computed ?  */
    if (n <= last_computed) return cache[n];

    /* ensure enough capacity */
    if (capacity <= (size_t)n) {
        size_t new_cap = capacity;
        /* double until we can hold index n */
        while (new_cap <= (size_t)n) new_cap <<= 1;
        seq_t *new_cache = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!new_cache) exit(1);
        cache    = new_cache;
        capacity = new_cap;
    }

    /* compute any missing terms up to n */
    for (idx_t i = last_computed + 1; i <= n; ++i) {
        seq_t val = (seq_t)3 * cache[i-1];   /* 3*a(i-1)   */
        val -= cache[i-3];                   /* minus a(i-3)*/
        val += (seq_t)3 * cache[i-4];        /* plus 3*a(i-4) */
        cache[i] = val;
    }

    last_computed = n;
    return cache[n];
}
