/*
 * A153886  a(n) = ((9 + sqrt(5))^n - (9 - sqrt(5))^n)/(2*sqrt(5)).
 * https://oeis.org/A153886
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 18, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) = 18*a(n-1) - 76*a(n-2), with a(0)=0, a(1)=1.
 * Offset: 1    (so a(1) is the first term listed below)
 */
#include <stdlib.h>

/* Compute A153886: a(n) = ((9 + sqrt(5))^n - (9 - sqrt(5))^n)/(2*sqrt(5)) */
seq_t a(idx_t n)
{
    /* handle trivial case */
    if (n == 0)
        return (seq_t)0;

    static seq_t *cache = NULL;
    static idx_t cache_size = 0;   /* highest index computed and stored */

    /* initial cache: a(0)=0, a(1)=1 */
    if (!cache) {
        cache = (seq_t*)malloc(2 * sizeof(seq_t));
        if (!cache) return (seq_t)0;      /* real program would handle error */
        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache_size = 1;
    }

    /* extend cache up to required n */
    if (n > cache_size) {
        seq_t *new_cache = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) return (seq_t)0;  /* error handling omitted */
        cache = new_cache;

        for (idx_t i = cache_size + 1; i <= n; ++i) {
            cache[i] = 18 * cache[i - 1] - 76 * cache[i - 2];
        }
        cache_size = n;
    }

    return cache[n];
}
