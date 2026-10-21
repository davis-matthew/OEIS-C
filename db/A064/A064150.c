/*
 * A064150  Numbers divisible by the sum of their ternary digits.
 * https://oeis.org/A064150
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn, nice
 * Formula:   a(n) = the n-th positive integer that is divisible by the sum of
 *            its ternary digits
 * Range:     u8 n <= 99, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1) return 0;                 /* undefined below offset */

    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;         /* number of terms stored   */
    static size_t cache_cap = 0;         /* allocated space         */
    static idx_t checked_up_to = 0;      /* last integer examined   */

    /* Generate terms until we have at least n of them */
    while (cache_len < n) {
        idx_t k = ++checked_up_to;       /* next integer to test    */

        /* sum of base‑3 digits of k */
        idx_t t = k;
        idx_t sum = 0;
        while (t) {
            sum += t % 3;
            t /= 3;
        }

        /* if k is divisible by its ternary digit sum, store it */
        if (sum != 0 && (k % sum) == 0) {
            if (cache_len == cache_cap) {             /* grow cache if necessary */
                size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                seq_t *new_cache = (seq_t*)realloc(cache,
                                                    new_cap * sizeof(seq_t));
                if (!new_cache) abort();              /* allocation failed */
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = (seq_t)k;
        }
    }

    return cache[n - 1];
}
