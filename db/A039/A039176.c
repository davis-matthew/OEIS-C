/*
 * A039176  Numbers whose base-11 representation has the same number of 2's
 *          and 5's.
 * https://oeis.org/A039176
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the nth integer whose base‑11 representation contains the
 *            same number of 2’s and 5’s (counting 0 as having zero of each)
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stddef.h>
#include "../utils/grow_seq.h"

seq_t a(idx_t n) {
    /* static cache of found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;            /* number of terms stored */
    static seq_t next_candidate = 0;         /* next integer to test */

    /* ensure we have at least n terms */
    while (cache_size < (size_t)n) {
        /* test next_candidate */
        seq_t x = next_candidate;
        if (x == 0) {                       /* 0 has 0 twos and 0 fives */
            if (cache_size == cache_cap)
                grow_seq(&cache, &cache_cap, cache_size + 1);
            cache[cache_size++] = x;
        } else {
            seq_t tmp = x;
            uint8_t count2 = 0, count5 = 0;
            while (tmp) {
                uint8_t digit = (uint8_t)(tmp % (seq_t)11);
                if (digit == 2) ++count2;
                else if (digit == 5) ++count5;
                tmp /= (seq_t)11;
            }
            if (count2 == count5) {
                if (cache_size == cache_cap)
                    grow_seq(&cache, &cache_cap, cache_size + 1);
                cache[cache_size++] = x;
            }
        }
        ++next_candidate;
    }
    /* n is 1‑based */
    return cache[(size_t)(n - 1)];
}
