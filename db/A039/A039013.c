/*
 * A039013  Numbers whose base-5 representation has the same number of 2's and
 *          3's.
 * https://oeis.org/A039013
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: enumerate numbers and test the base-5 digit condition */
/* Sequence: Numbers whose base‑5 representation has an equal number of 2's and 3's. */

#include <stdlib.h>
#include <stdbool.h>

static bool is_balanced(seq_t val) {
    seq_t tmp = val;
    unsigned int cnt2 = 0, cnt3 = 0;
    while (tmp != 0) {
        unsigned int digit = (unsigned int)(tmp % (seq_t)5);
        if (digit == 2) cnt2++;
        else if (digit == 3) cnt3++;
        tmp /= (seq_t)5;
    }
    return cnt2 == cnt3;
}

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static idx_t cache_size = 0;
    static idx_t cache_capacity = 0;
    static idx_t next_checked = 0;

    while (cache_size < n) {
        if (is_balanced((seq_t)next_checked)) {
            if (cache_size == cache_capacity) {
                size_t new_cap = cache_capacity == 0 ? 16 : cache_capacity * 2;
                cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                cache_capacity = new_cap;
            }
            cache[cache_size++] = (seq_t)next_checked;
        }
        next_checked++;
    }

    return cache[n - 1];
}
