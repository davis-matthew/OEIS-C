/*
 * A231101  a(n) = 3*a(n-3) + a(n-2), a(0)=3, a(1)=0, a(2)=2.
 * https://oeis.org/A231101
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-3)+a(n-2), a(0)=3, a(1)=0, a(2)=2
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 43, u64 n <= 86, u128 n <= 172,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the three explicit base cases */
    if (n == 0) return (seq_t)3;
    if (n == 1) return (seq_t)0;
    if (n == 2) return (seq_t)2;

    /* Static storage for memoisation */
    static seq_t *cache      = NULL;
    static size_t cache_cap  = 0;
    static size_t max_computed = 2;  /* index of highest value computed */

    /* Initialise cache on first use */
    if (cache == NULL) {
        cache_cap = (size_t)n + 1;
        cache = (seq_t *)malloc(sizeof(seq_t) * cache_cap);
        if (cache == NULL) { /* allocation failure – best effort */ }
        cache[0] = (seq_t)3;
        cache[1] = (seq_t)0;
        cache[2] = (seq_t)2;
    }

    /* Grow capacity if necessary */
    if (cache_cap <= (size_t)n) {
        size_t new_cap = cache_cap;
        if (new_cap == 0) new_cap = 8;
        while (new_cap <= (size_t)n) new_cap <<= 1;  /* double */
        seq_t *tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_cap);
        if (tmp == NULL) { /* allocation failure – best effort */ }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute any new terms that are required */
    if ((size_t)n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            cache[(size_t)i] = (seq_t)3 * cache[(size_t)(i - 3)] + cache[(size_t)(i - 2)];
        }
        max_computed = (size_t)n;
    }

    return cache[(size_t)n];
}
