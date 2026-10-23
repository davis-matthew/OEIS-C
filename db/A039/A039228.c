/*
 * A039228  Numbers whose base-12 representation has the same number of 1's
 *          and 10's.
 * https://oeis.org/A039228
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 110, u16 n <= 5000, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>

/* Calculation: brute‑force search for numbers whose base‑12 representation
 * contains equal numbers of digits 1 and 10. */
seq_t a(idx_t n)
{
    /* Static cache of previously found terms */
    static seq_t *cache = NULL;
    static idx_t cache_cnt = 0;      /* number of cached terms */
    static idx_t next_int = 0;       /* next integer to test */

    if (n == 0) return 0;  /* undefined for 0, but return 0 */

    /* Grow cache until it contains at least n terms */
    if (cache_cnt < n) {
        /* Determine new capacity (double or at least n) */
        size_t new_cap = (cache == NULL) ? 16 : cache_cnt * 2;
        if (new_cap < (size_t)n) new_cap = (size_t)n;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        /* Find numbers until the cache is full up to n */
        while (cache_cnt < n) {
            idx_t x = next_int;
            idx_t tmp = x;
            size_t cnt1 = 0, cnt10 = 0;
            /* Handle 0 correctly using a do/while loop */
            do {
                const idx_t d = tmp % 12;
                if (d == 1) ++cnt1;
                else if (d == 10) ++cnt10;
                tmp /= 12;
            } while (tmp);
            if (cnt1 == cnt10) {
                cache[cache_cnt++] = (seq_t) x;
            }
            ++next_int;
        }
    }
    return cache[n - 1];
}
