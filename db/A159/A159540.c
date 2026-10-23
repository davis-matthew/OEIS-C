/*
 * A159540  Numerator of Hermite(n, 12/17).
 * https://oeis.org/A159540
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = 24*a(n-1) - 578*(n-1)*a(n-2) (n ≥ 2), a(0)=1, a(1)=24
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 5, i64 n <= 11, i128 n <= 20,
 *            big any n
 * Helpers:   free_seq_array, seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/seq_alloc_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    /* Static cache for memoised results */
    static seq_t *cache = NULL;          /* array of computed values      */
    static idx_t  cache_len = 0;         /* current length of the array  */
    static idx_t  computed_upto = 0;     /* highest index computed so far */

    /* Initialise cache with the first two terms if it does not exist yet */
    if (cache == NULL) {
        cache_len = 2;
        cache = seq_alloc_array(cache_len);
        cache[0] = 1;
        cache[1] = 24;
        computed_upto = 1;
    }

    /* Grow the cache if necessary */
    if (n >= cache_len) {
        idx_t new_len = n + 1;
        seq_t *new_cache = seq_alloc_array(new_len);
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];
        free_seq_array(cache);
        cache = new_cache;
        cache_len = new_len;
    }

    /* Compute missing values up to n (if needed) */
    if (n > computed_upto) {
        for (idx_t i = computed_upto + 1; i <= n; ++i) {
            seq_t term_mul = (seq_t)578 * (seq_t)(i - 1);
            cache[i] = 24 * cache[i - 1] - term_mul * cache[i - 2];
        }
        computed_upto = n;
    }

    return cache[n];
}
