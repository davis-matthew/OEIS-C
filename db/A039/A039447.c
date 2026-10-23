/*
 * A039447  Numbers whose base-11 representation has the same nonzero number
 *          of 0's and 10's.
 * https://oeis.org/A039447
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: find the nth integer whose base‑11 representation has an equal
   non‑zero number of digits 0 and 10. */
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache   = NULL;
    static size_t cache_len = 0, cache_cap = 0;
    static idx_t last_checked = 0;

    if (n == 0) return 0;  /* undefined for n < 1, return 0 */

    while (cache_len < (size_t)n) {
        last_checked++;
        seq_t val = last_checked;
        size_t zero = 0, ten = 0;
        seq_t tmp = val;
        while (tmp > 0) {
            seq_t digit = tmp % 11;
            if (digit == 0) zero++;
            else if (digit == 10) ten++;
            tmp /= 11;
        }
        if (zero > 0 && zero == ten) {
            if (cache_len == cache_cap) {
                size_t new_cap = cache_cap ? cache_cap * 2 : 16;
                seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!new_cache) exit(1);  /* allocation failed */
                cache = new_cache;
                cache_cap = new_cap;
            }
            cache[cache_len++] = val;
        }
    }
    return cache[(size_t)n - 1];
}
