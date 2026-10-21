/*
 * A214100  Number of 0..2 colorings on an nX8 array circular in the 8
 *          direction with new values 0..2 introduced in row major order.
 * https://oeis.org/A214100
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 54*a(n-1) - 820*a(n-2) + 4906*a(n-3) - 11803*a(n-4) +
 *            11888*a(n-5) - 4672*a(n-6) + 576*a(n-7) with a(1)=43, a(2)=1179,
 *            a(3)=37441, a(4)=1241355, a(5)=41634729, a(6)=1400634371,
 *            a(7)=47155766193. The implementation memoises computed terms in
 *            a static cache
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 12, u128 n <= 25,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of computed values: cache[0] = a(1) */
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;   /* number of values currently stored */
    static idx_t cache_cap = 0;   /* capacity of the cache array */

    if (n == 0) {
        /* The sequence is defined for n >= 1; for n = 0 return 0. */
        return (seq_t)0;
    }

    /* Ensure the cache has enough capacity for the requested n */
    if (cache == NULL || cache_cap < n) {
        idx_t new_cap = n;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (tmp == NULL) {
            abort();
        }
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Compute values up to n, filling the cache */
    for (idx_t i = cache_len + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = (seq_t)43;
        } else if (i == 2) {
            val = (seq_t)1179;
        } else if (i == 3) {
            val = (seq_t)37441;
        } else if (i == 4) {
            val = (seq_t)1241355;
        } else if (i == 5) {
            val = (seq_t)41634729;
        } else if (i == 6) {
            val = (seq_t)1400634371;
        } else if (i == 7) {
            val = (seq_t)47155766193;
        } else {
            /* Recurrence for i >= 8:
             * a(i) = 54*a(i-1) - 820*a(i-2) + 4906*a(i-3) 
             *        - 11803*a(i-4) + 11888*a(i-5) 
             *        - 4672 *a(i-6) + 576*a(i-7)               */
            idx_t j = i - 1;          /* 0‑based index of a(i) */
            /* Compute positive contributions first */
            seq_t sum = (seq_t)((__int128)54   * cache[j - 1]);   /* 54*a(i-1) */
            sum += (seq_t)((__int128)4906 * cache[j - 3]);      /* +4906*a(i-3) */
            sum += (seq_t)((__int128)11888 * cache[j - 5]);     /* +11888*a(i-5) */
            sum += (seq_t)((__int128)576 * cache[j - 7]);       /* +576*a(i-7) */

            /* Subtract negative contributions; the result is guaranteed
               to be non‑negative so unsigned subtraction is safe. */
            sum -= (seq_t)((__int128)820 * cache[j - 2]);       /* -820*a(i-2) */
            sum -= (seq_t)((__int128)11803 * cache[j - 4]);     /* -11803*a(i-4) */
            sum -= (seq_t)((__int128)4672 * cache[j - 6]);      /* -4672*a(i-6) */

            val = sum;
        }
        cache[i - 1] = val;
    }
    cache_len = (cache_len < n) ? n : cache_len;

    return cache[n - 1];
}
