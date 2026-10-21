/*
 * A077868  Expansion of 1/((1-x)*(1-x-x^3)).
 * https://oeis.org/A077868
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + a(n-1) + a(n-3) for n ≥ 3, with a(0)=1, a(1)=2,
 *            a(2)=3
 * Range:     u8 n <= 12, u16 n <= 27, u32 n <= 56, u64 n <= 114,
 *            u128 n <= 230, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache storing already computed terms */
    static seq_t *cache = NULL;          /* array of terms */
    static size_t capacity = 0;          /* current allocated size */

    /* bootstrap the cache with the first three values */
    if (capacity == 0) {
        capacity = 3;
        cache = (seq_t*)malloc(capacity * sizeof(seq_t));
        if (cache == NULL) return 0;     /* out of memory, unlikely for valid tests */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)3;
    }

    /* If n is beyond current capacity, extend the array and fill new values */
    if ((size_t)n >= capacity) {
        size_t new_cap = (size_t)n + 1;  /* enough for index n */
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) return 0;       /* allocation failure */
        cache = tmp;
        /* compute all terms that were not yet computed */
        for (size_t i = capacity; i <= n; ++i)
            cache[i] = (seq_t)1 + cache[i-1] + cache[i-3];
        capacity = new_cap;
    }

    /* For n already within the computed range, just return the stored value */
    return cache[(size_t)n];
}
