/*
 * A170934  a(n) = b(n) + b(n+1) + 2, where b() = A000930().
 * https://oeis.org/A170934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-3) - 2 with a(0) = 4, a(1) = 4, a(2) = 5
 * Range:     u8 n <= 13, u16 n <= 27, u32 n <= 56, u64 n <= 114,
 *            u128 n <= 231, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* array of computed values   */
    static idx_t  cap   = 0;          /* current allocated capacity */
    static idx_t  max_computed = 2;   /* highest index already computed */

    /* initialise cache on first call */
    if (cache == NULL) {
        cap = 4;                      /* store indices 0…3 at least          */
        cache = (seq_t *)calloc((size_t)cap, sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure */
        cache[0] = (seq_t)4;
        cache[1] = (seq_t)4;
        cache[2] = (seq_t)5;
        /* cache[3] will be computed on demand */
    }

    /* grow the cache if we need to store more terms   */
    if (n + 1 > cap) {
        idx_t new_cap = cap;
        while (new_cap <= n) new_cap <<= 1;  /* double until > n  */
        seq_t *tmp = (seq_t *)realloc(cache, (size_t)new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;   /* allocation failure */
        cache = tmp;
        cap = new_cap;
    }

    /* compute any missing terms up to n   */
    if (n > max_computed) {
        for (idx_t i = max_computed + 1; i <= n; ++i) {
            cache[i] = cache[i-1] + cache[i-3] - (seq_t)2;
        }
        max_computed = n;
    }

    return cache[n];
}
