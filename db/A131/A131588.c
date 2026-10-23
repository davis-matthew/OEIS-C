/*
 * A131588  Interlaces A007583 with A083420.
 * https://oeis.org/A131588
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-2) - 4*a(n-4); Initial values: a(0) = 1, a(1) = 1,
 *            a(2) = 3, a(3) = 7 (Derived recurrence from the closed‑form
 *            expression)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* static cache for memoisation */
static seq_t *cache = NULL;      /* array a[0] .. a[max] */
static idx_t cache_len = 0;      /* number of valid entries */

seq_t a(idx_t n)
{
    /* enlarge cache if needed */
    if (n >= cache_len) {
        seq_t *new_cache = (seq_t *)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!new_cache) {
            /* allocation failure – abort the program (no other safe action) */
            abort();
        }
        cache = new_cache;

        /* compute all missing terms */
        for (idx_t i = cache_len; i <= n; ++i) {
            seq_t val;
            if (i == 0)        val = 1;
            else if (i == 1)   val = 1;
            else if (i == 2)   val = 3;
            else if (i == 3)   val = 7;
            else               val = 5 * cache[i - 2] - 4 * cache[i - 4];
            cache[i] = val;
        }
        cache_len = n + 1;
    }
    return cache[n];
}
