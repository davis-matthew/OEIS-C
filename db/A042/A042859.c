/*
 * A042859  Denominators of continued fraction convergents to sqrt(960).
 * https://oeis.org/A042859
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 11, u64 n <= 21, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Denominators of continued fraction convergents to sqrt(960)
   OEIS A042859
   Recurrence: a(0)=1, a(1)=1, a(2)=61, a(3)=62
               a(n) = 62*a(n-2) - a(n-4)   for n > 3
*/

#include <stdlib.h>
#include <string.h>   /* for memcpy if needed */

seq_t a(idx_t n)
{
    /* static cache of computed values (lazy allocation) */
    static seq_t *cache          = NULL;
    static idx_t cache_len       = 0;   /* current allocated size   */
    static idx_t last_filled     = 0;   /* highest index that has a valid value */
    static int   init_flag       = 0;   /* 0 => not initialized yet */

    /* ---------------------------------------------------------
       Ensure the cache is large enough for indices up to n
       --------------------------------------------------------- */

    if (n >= cache_len) {
        idx_t need = n + 1;
        if (need < 4) need = 4;              /* at least 4 slots for the initial terms */

        /* Allocate (or reallocate) the cache buffer */
        seq_t *new_cache = (seq_t *)malloc(need * sizeof(seq_t));
        if (new_cache == NULL) {
            /* Allocation failed: fall back to a local temporary array
               and compute the requested value without caching.
               This path is unlikely to be hit in normal use. */
            seq_t v[4] = { (seq_t)1, (seq_t)1, (seq_t)61, (seq_t)62 };
            if (n < 4) return v[n];
            for (idx_t i = 4; i <= n; ++i) {
                seq_t next = (seq_t)62 * v[(i - 2) % 4] - v[(i - 4) % 4];
                v[i % 4] = next;
            }
            return v[n % 4];
        }

        /* Copy old values to the new buffer (avoiding memcpy which may
           corrupt non‑trivial types). */
        for (idx_t i = 0; i < cache_len; ++i)
            new_cache[i] = cache[i];

        /* Free old buffer and update pointers */
        if (cache != NULL) free(cache);
        cache      = new_cache;
        cache_len  = need;

        /* If this is a fresh allocation, initialize the first four terms */
        if (!init_flag) {
            cache[0] = (seq_t)1;
            cache[1] = (seq_t)1;
            cache[2] = (seq_t)61;
            cache[3] = (seq_t)62;
            init_flag = 1;
            last_filled = 3;
        }
    }

    /* ----- Initialize the cache if this is the first call ***************** */
    if (!init_flag) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)61;
        cache[3] = (seq_t)62;
        init_flag = 1;
        last_filled = 3;
    }

    /* ----- Compute missing values up to n, if necessary ******************* */
    if (n > last_filled) {
        static const seq_t sixty_two = (seq_t)62;
        for (idx_t i = last_filled + 1; i <= n; ++i) {
            /* recurrence: a[i] = 62 * a[i-2] - a[i-4] */
            cache[i] = sixty_two * cache[i - 2] - cache[i - 4];
        }
        last_filled = n;
    }

    return cache[n];
}
