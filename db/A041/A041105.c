/*
 * A041105  Denominators of continued fraction convergents to sqrt(60).
 * https://oeis.org/A041105
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, frac
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 23, u64 n <= 43, u128 n <= 87,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: A041105 – denominators of continued fraction convergents to sqrt(60)
   Recurrence: a(n) = 62*a(n-4) - a(n-8),  n ≥ 8
   Initial values:
        a(0)=1, a(1)=1, a(2)=3, a(3)=4,
        a(4)=59, a(5)=63, a(6)=185, a(7)=248
   This implementation caches computed values and extends the cache only
   when needed.  It works for arbitrary‐precision seq_t as well as for
   fixed‐width unsigned types. */
#include <stdlib.h>
#include <stddef.h>

static const seq_t init_vals[8] = {
    1, 1, 3, 4, 59, 63, 185, 248
};

seq_t a(idx_t n)
{
    /* cache for computed values, grows as needed */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;

    if (n < 8)
        return init_vals[(size_t)n];

    /* initialise cache with the first eight values if not already done */
    if (cache_len == 0) {
        cache_len = 8;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) return 0;               /* allocation failure – should not happen */
        for (size_t i = 0; i < 8; ++i)
            cache[i] = init_vals[i];
    }

    size_t idx = (size_t)n;
    if (idx >= cache_len) {
        size_t old_len = cache_len;
        size_t new_len = idx + 1;          /* include idx itself */

        if (new_len > cache_len) {
            seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
            if (!tmp) return 0;            /* allocation failure – should not happen */
            cache = tmp;
            cache_len = new_len;
        }

        /* compute missing terms using the recurrence */
        for (size_t k = old_len; k <= idx; ++k) {
            cache[k] = 62 * cache[k - 4] - cache[k - 8];
        }
    }

    return cache[idx];
}
