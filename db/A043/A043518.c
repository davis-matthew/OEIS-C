/*
 * A043518  Numbers having two 7's in base 10.
 * https://oeis.org/A043518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3056, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers having exactly two 7's in base 10.
   a(n) is the n-th positive integer whose decimal representation
   contains exactly two digits '7'.
   This implementation scans integers in order, counts the '7's,
   and caches already found terms so subsequent calls are fast. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached terms  a(1)...a(cache_len) */
    static idx_t cache_len = 0;   /* number of terms cached              */
    static size_t cache_cap = 0;  /* capacity of the cache array         */

    if (cache_len >= n)
        return cache[n - 1];     /* already computed */

    /* start counting from the next integer after the last cached one */
    seq_t x = cache_len ? cache[cache_len - 1] + 1 : 0;
    while (cache_len < n) {
        /* count how many '7's are in x */
        seq_t tmp = x;
        int cnt7 = 0;
        while (tmp > 0) {
            int d = (int)(tmp % 10);
            if (d == 7) {
                ++cnt7;
                if (cnt7 > 2)   /* cannot be a term */
                    break;
            }
            tmp /= 10;
        }
        if (cnt7 == 2) {
            /* grow cache if needed */
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) exit(1);  /* allocation failed */
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = x;
        }
        ++x;
    }
    return cache[n - 1];
}
