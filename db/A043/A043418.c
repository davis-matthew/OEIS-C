/*
 * A043418  Numbers whose base-7 representation contains exactly two 6's.
 * https://oeis.org/A043418
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers whose base-7 representation contains exactly two 6's */
#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    static seq_t candidate = 0;

    while (cache_size < n) {
        unsigned cnt = 0;
        seq_t tmp = candidate;
        while (tmp) {
            if (tmp % 7 == 6) {
                ++cnt;
                if (cnt > 2) break;
            }
            tmp /= 7;
        }
        if (cnt == 2) {
            if (cache_size >= cache_cap) {
                size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
                while (new_cap <= cache_size) new_cap *= 2;
                seq_t *new_ptr = (seq_t *)realloc(cache,
                                                  new_cap * sizeof(seq_t));
                if (!new_ptr) abort();
                cache = new_ptr;
                cache_cap = new_cap;
            }
            cache[cache_size++] = candidate;
        }
        ++candidate;
    }
    return cache[n - 1];
}
