/*
 * A084150  A Pell related sequence.
 * https://oeis.org/A084150
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ((1+sqrt(8))^n + (1-sqrt(8))^n - 2)/16 Recurrence:
 *            a(0)=0, a(1)=0, a(n)=2*a(n-1)+7*a(n-2)+1 for n>=2 Uses O(n) time
 *            with memoisation
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 18, u64 n <= 35, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;       /* memoisation table */
    static idx_t cache_len = 0;       /* current number of stored entries (= last_index+1) */

    if (cache_len <= n) {                     /* need to compute up to n */
        size_t new_len = (size_t)(n + 1);      /* size of the new array */
        if (cache == NULL) {
            cache = (seq_t *)malloc(new_len * sizeof(seq_t));
        } else {
            cache = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
        }
        /* if allocation failed, abort the program */
        if (cache == NULL) abort();

        /* initialise base values if not already present */
        if (cache_len == 0) {
            cache[0] = 0;
        }
        if (cache_len <= 1 && new_len > 1) {
            cache[1] = 0;
        }

        /* compute values from cache_len to n using the recurrence */
        idx_t start = cache_len;                     /* first index to compute */
        if (n >= 2) {
            for (idx_t i = (start > 1 ? start : (idx_t)2); i <= n; ++i) {
                cache[i] = cache[i-1] * (seq_t)2 + cache[i-2] * (seq_t)7 + (seq_t)1;
            }
        }

        cache_len = n + 1;    /* update number of stored entries */
    }

    return cache[n];
}
