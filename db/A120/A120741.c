/*
 * A120741  a(n) = (7^n - 1)/2.
 * https://oeis.org/A120741
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (7^n - 1)/2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 45,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0) = 0, a(1) = 3, a(n) = 8*a(n-1) - 7*a(n-2) for n >= 2 */

#include <stddef.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Static cache: stores terms already computed */
    static seq_t *cache   = NULL; /* array of seq_t values */
    static size_t cap     = 0;    /* capacity of the cache array   */
    static idx_t  computed = 0;   /* number of values filled       */

    /* initialise cache with room for at least the first two terms */
    if (cap == 0) {
        cap = 2;
        cache = seq_alloc(cap);            /* allocates an array of seq_t */
        if (!cache) return 0;              /* allocation failure – sentinel */
        cache[0] = 0;                      /* a(0) = 0 */
        cache[1] = 3;                      /* a(1) = 3 */
        computed = 2;
    }

    /* grow cache if needed (doubling strategy) */
    if ((idx_t)(n + 1) > (idx_t)cap) {
        size_t new_cap = cap * 2;
        if ((idx_t)new_cap < (idx_t)(n + 1))
            new_cap = n + 1;
        seq_t *tmp = seq_realloc(cache, cap, new_cap);
        if (!tmp) return 0;                  /* allocation failure – sentinel */
        cache = tmp;
        cap = new_cap;
    }

    /* compute any missing terms up to index n */
    if (computed <= n) {
        for (idx_t i = computed; i <= n; ++i) {
            if (i == 0) {
                cache[i] = 0;
            } else if (i == 1) {
                cache[i] = 3;
            } else {
                /* recurrence: a(n) = 8*a(n-1) – 7*a(n-2) */
                cache[i] = cache[i-1] * 8 - cache[i-2] * 7;
            }
        }
        computed = n + 1;
    }

    return cache[n];
}
