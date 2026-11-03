/*
 * A122502  Expansion of x/(1 - 22*x^2 - 54*x^3 - 38*x^4).
 * https://oeis.org/A122502
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 14, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Validated: partial, pass, 22 terms through n = 21 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(0)=0, a(1)=1, a(2)=0, a(3)=22,
   a(n)=22*a(n-2)+54*a(n-3)+38*a(n-4) */

#include <stdlib.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;      /* memoized values */
    static idx_t cache_len = 0;      /* number of cached entries */

    /* first call: allocate space for at least 4 entries */
    if (!cache) {
        size_t init = 4;
        if ((size_t)n + 1 > init) init = (size_t)n + 1;
        cache = mem_new(seq_t, init);
        if (!cache) return (seq_t)0; /* allocation failure */

        cache[0] = (seq_t)0;
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)22;
        cache_len = (idx_t)init;

        if (n < 4) return cache[n];
    }

    /* enlarge cache if needed */
    if (n >= cache_len) {
        idx_t old_len = cache_len;
        idx_t new_len = n + 1;
        seq_t *tmp = mem_resize(seq_t, cache, (size_t)new_len);
        if (!tmp) return (seq_t)0; /* allocation failure */
        cache = tmp;
        cache_len = new_len;

        /* compute new values */
        size_t start = 4;
        if ((size_t)old_len > start) start = (size_t)old_len;
        for (size_t i = start; i <= (size_t)n; ++i) {
            cache[i] = (seq_t)22 * cache[i - 2] + (seq_t)54 * cache[i - 3] + (seq_t)38 * cache[i - 4];
        }
    }

    return cache[n];
}
