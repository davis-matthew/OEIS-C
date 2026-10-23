/*
 * A159654  Numerator of Hermite(n, 16/19).
 * https://oeis.org/A159654
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   not stated
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 10, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A159654: Numerator of Hermite(n, 16/19)
   Recurrence:
       a(0) = 1
       a(1) = 32
       a(n) = 32*a(n-1) - 722*(n-1)*a(n-2)   (for n ≥ 2)
   The function keeps a static cache so that calls with arbitrary indices
   are answered in O(n) time overall, even if the indices arrive out of
   order.
*/
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed terms */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* allocated length of cache array   */
    static idx_t last_computed = (idx_t)-1;   /* largest index computed */

    /* allocate initial cache space if needed */
    if (cache == NULL) {
        cache_len = 16;  /* arbitrary starting size */
        cache = (seq_t*)malloc(cache_len * sizeof(seq_t));
        if (cache == NULL) return 0;  /* malloc failed; return 0 as error code    */
        cache[0] = (seq_t)1;
        cache_len = 1;
        if (n == 0) { last_computed = 0; return cache[0]; }
        /* prepare for a(1) if requested */
        if (cache_len < 2) {
            /* grow to size at least 2 */
            cache_len = 2;
            cache = (seq_t*)realloc(cache, cache_len * sizeof(seq_t));
            if (cache == NULL) return 0;
        }
        cache[1] = (seq_t)32;
        last_computed = 1;
        if (n == 1) return cache[1];
    }

    /* grow cache if we need more space for n */
    if (n >= cache_len) {
        idx_t new_len = cache_len;
        while (new_len <= n) new_len *= 2;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (tmp == NULL) return 0;  /* realloc failed */
        cache = tmp;
        cache_len = new_len;
    }

    /* compute missing terms up to n */
    if (n > last_computed) {
        for (idx_t i = last_computed + 1; i <= n; ++i) {
            seq_t term1 = (seq_t)32 * cache[i - 1];
            seq_t term2 = (seq_t)722 * (seq_t)(i - 1) * cache[i - 2];
            cache[i] = term1 - term2;
        }
        last_computed = n;
    }

    return cache[n];
}
