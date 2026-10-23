/*
 * A039557  Numbers whose base-12 representation has the same nonzero number
 *          of 9's and 11's.
 * https://oeis.org/A039557
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers whose base‑12 representation contains the same non‑zero
 *            number of 9’s and 11’s (digits 9 and B). Sequence enumerated in
 *            increasing order
 * Range:     u8 n <= 2, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n) {
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_size = 0;
    static size_t cache_capacity = 0;
    /* next candidate to test */
    static seq_t cand = 1;

    while (cache_size < (size_t)n) {
        seq_t x = cand;
        int cnt9 = 0, cnt11 = 0;
        if (x != 0) {
            while (x != 0) {
                int d = (int)(x % (seq_t)12);
                if (d == 9) cnt9++;
                else if (d == 11) cnt11++;
                x /= (seq_t)12;
            }
        }
        if (cnt9 > 0 && cnt9 == cnt11) {
            if (cache_size == cache_capacity) {
                cache_capacity = cache_capacity ? cache_capacity * 2 : 16;
                cache = (seq_t *)realloc(cache, cache_capacity * sizeof(seq_t));
            }
            cache[cache_size++] = cand;
        }
        cand++;
    }
    return cache[n-1];
}
