/*
 * A173770  a(n) = (4*10^n - 13)/9.
 * https://oeis.org/A173770
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*10^n - 13)/9 * Recurrence: a(1) = 3 * a(n) = 10*a(n-1)
 *            + 13 for n > 1
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return (seq_t)0;          /* undefined below offset */

    /* Memoization table that grows as needed. */
    static seq_t *cache = NULL;
    static size_t cap = 0;               /* cache holds indices 0..cap-1 */

    /* Make sure the cache covers index n. */
    if ((size_t)n + 1 > cap) {
        size_t newcap = (size_t)n + 1;
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) {                      /* allocation failure */
            return (seq_t)0;
        }
        cache = tmp;

        /* Initialise new entries. */
        if (cap == 0) {
            /* First allocation: set a(1). */
            if (newcap > 1) cache[1] = (seq_t)3;
        } else {
            /* Continue recurrence from cache[cap-1] up to cache[newcap-1]. */
            seq_t ten = (seq_t)10;
            seq_t thirteen = (seq_t)13;
            for (size_t i = cap; i < newcap; ++i)
                cache[i] = cache[i - 1] * ten + thirteen;
        }
        cap = newcap;
    }

    return cache[n];
}
