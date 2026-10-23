/*
 * A043843  Numbers n such that number of runs in the base 4 representation of
 *          n is congruent to 0 mod 7.
 * https://oeis.org/A043843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth integer such that the number of runs in its
 *            base‑4 representation is congruent to 0 modulo 7. The value is
 *            computed by a simple linear search with memoisation of
 *            previously found terms
 * Range:     u8 none, u16 n <= 36, u32 n <= 36, u64 n <= 36, u128 n <= 36,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;   /* undefined for index 0, just return 0 */

    /* static cache for already found terms */
    static seq_t *cache      = NULL;   /* array of found terms */
    static size_t  cache_len = 0;     /* number of terms stored */
    static size_t  cache_cap = 0;     /* allocated capacity */
    static idx_t  next_checked = 0;   /* highest int already examined */

    /* initialise cache on first call */
    if (cache_cap == 0) {
        cache_cap = 64;
        cache = (seq_t*)malloc(sizeof(seq_t) * cache_cap);
    }

    /* extend cache if necessary */
    while (cache_len < n) {
        /* search for next integer that satisfies the condition */
        ++next_checked;
        idx_t num = next_checked;

        /* count runs in base‑4 representation */
        int runs = 0;
        int last_digit = -1;
        idx_t s = num;
        while (s > 0) {
            int d = (int)(s % 4);
            if (d != last_digit) {
                ++runs;
                last_digit = d;
            }
            s /= 4;
        }
        /* if number of runs ≡ 0 (mod 7), store it */
        if (runs % 7 == 0) {
            if (cache_len == cache_cap) {
                cache_cap *= 2;
                cache = (seq_t*)realloc(cache, sizeof(seq_t) * cache_cap);
            }
            cache[cache_len++] = (seq_t)num;
        }
    }
    return cache[n - 1];
}
