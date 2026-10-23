/*
 * A183784  Half the number of (n+1) X 2 0..2 arrays with no 2 X 2 subblock
 *          having sum 4.
 * https://oeis.org/A183784
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*a(n-1) + 24*a(n-2) + 24*a(n-3) Initial values: a(1)=31,
 *            a(2)=216, a(3)=1500 This uses an O(n) recurrence with
 *            memoisation
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 10, u64 n <= 22, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_len = 0;   /* number of allocated slots (index 0 unused) */

    if (n < 1) return 0;          /* undefined below the offset */

    size_t idx = (size_t)n;

    /* grow cache if necessary */
    if (idx + 1 > cache_len) {
        size_t newcap = idx + 1;
        if (newcap < 4) newcap = 4;   /* at least three terms plus 0 */

        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) { /* allocation failure – terminate the program */
            exit(EXIT_FAILURE);
        }
        cache = tmp;

        /* initialise newly allocated entries */
        for (size_t i = cache_len; i < newcap; ++i) {
            if (i == 0) continue;            /* skip unused slot 0 */
            if (i == 1) cache[i] = (seq_t)31;
            else if (i == 2) cache[i] = (seq_t)216;
            else if (i == 3) cache[i] = (seq_t)1500;
            else
                cache[i] = cache[i-1] * (seq_t)3
                         + cache[i-2] * (seq_t)24
                         + cache[i-3] * (seq_t)24;
        }
        cache_len = newcap;
    }

    return cache[idx];
}
