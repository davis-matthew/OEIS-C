/*
 * A066138  a(n) = 10^(2*n) + 10^n + 1.
 * https://oeis.org/A066138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = 10^(2*n) + 10^n + 1 Recurrence: a(0)=3, a(1)=111,
 *            a(2)=10101, a(n)=111*a(n-1)-1110*a(n-2)+1000*a(n-3) for n≥3
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of items stored */

    /* Ensure the cache can hold indices up to n */
    if (cache_len <= n) {
        idx_t old_len = cache_len;
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) { /* allocation failed – terminate gracefully */
            abort();
        }
        cache = tmp;
        cache_len = new_len;

        /* Compute missing entries using the recurrence */
        for (idx_t i = old_len; i <= n; ++i) {
            seq_t val;
            if (i == 0)
                val = 3;
            else if (i == 1)
                val = 111;
            else if (i == 2)
                val = 10101;
            else
                val = 111 * cache[i - 1] - 1110 * cache[i - 2] + 1000 * cache[i - 3];
            cache[i] = val;
        }
    }

    return cache[n];
}
