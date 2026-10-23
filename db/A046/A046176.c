/*
 * A046176  Indices of square numbers that are also hexagonal.
 * https://oeis.org/A046176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 34*a(n-1) - a(n-2); a(1) = 1, a(2) = 35
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 13, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static idx_t cache_len = 0;

    if (n == 0) return 0; /* not defined for n < 1 */

    if (cache_len < n) {
        size_t new_len = (size_t)n;
        seq_t *tmp = (seq_t*)realloc(cache, new_len * sizeof(seq_t));
        if (!tmp) return 0; /* allocation failure – unlikely in tests */
        cache = tmp;

        if (cache_len == 0) {
            cache[0] = (seq_t)1;               /* a(1) */
            cache_len = 1;
        }
        if (cache_len == 1 && new_len >= 2) {
            cache[1] = (seq_t)35;               /* a(2) */
            cache_len = 2;
        }
        for (idx_t i = cache_len + 1; i <= (idx_t)new_len; ++i) {
            /* recurrence: a(i) = 34*a(i-1) - a(i-2) */
            cache[i-1] = (seq_t)((seq_t)34 * cache[i-2] - cache[i-3]);
        }
        cache_len = new_len;
    }

    return cache[n-1];
}
