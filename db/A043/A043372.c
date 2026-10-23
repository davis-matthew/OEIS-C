/*
 * A043372  Numbers having four 0's in base 6.
 * https://oeis.org/A043372
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 35, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*
 * Sequence: numbers having exactly four 0's in base‑6 representation.
 * Computed by brute‑force search with memoisation.
 */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n) {
    /* static cache for memoisation */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;
    static size_t cache_size = 0;
    static seq_t next_val = 1;       /* next integer to be examined */

    if (n == 0) return 0;            /* safety: indices start at 1 */

    while (cache_size < n) {
        /* Count zeros in the base‑6 representation of next_val */
        seq_t v = next_val;
        unsigned short zero_cnt = 0;
        while (v > 0) {
            if ((v % 6) == 0) ++zero_cnt;
            v /= 6;
        }

        if (zero_cnt == 4) {
            /* Ensure capacity and store the found value */
            if (cache_size == cache_cap) {
                size_t new_cap = cache_cap == 0 ? 16 : cache_cap * 2;
                seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;   /* allocation failed (unlikely) */
                cache = tmp;
                cache_cap = new_cap;
            }
            cache[cache_size++] = next_val;
        }

        ++next_val;   /* move to the next integer to test */
    }

    return cache[n - 1];
}
