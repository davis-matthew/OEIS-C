/*
 * A080734  a(1)=1, then a(n)=a(n-1)+2 if the final decimal digit of a(n) is
 *          0, a(n)=a(n-1)+3 otherwise.
 * https://oeis.org/A080734
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 61, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1; a(n)=a(n-1)+2 if a(n-1) ends with 0 else a(n-1)+3 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Lazy memoization of computed values.  cache[0] is unused. */
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;

    if (n == 0) return 0;  /* undefined, but safe */

    /* Grow cache to accommodate n */
    if (n > cache_size) {
        /* Allocate an array large enough for indices up to n (1‑based). */
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) return 0;      /* allocation failed; return 0 as sentinel */
        cache = new_cache;

        /* First element after a(1) must be initialized */
        if (cache_size == 0) {
            cache[1] = (seq_t)1;
            cache_size = 1;
        }

        /* Compute missing values */
        for (idx_t i = cache_size + 1; i <= n; ++i) {
            seq_t prev = cache[i - 1];
            /* Check last decimal digit.  prev%10 works for all unsigned seq_t types. */
            if ((prev % (seq_t)10) == 0)
                cache[i] = prev + (seq_t)2;
            else
                cache[i] = prev + (seq_t)3;
        }
        cache_size = n;
    }
    return cache[n];
}
