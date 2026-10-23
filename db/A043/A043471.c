/*
 * A043471  Numbers having three 4's in base 9.
 * https://oeis.org/A043471
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 37, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: numbers whose base‑9 representation contains exactly three 4's. */
#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array of found terms */
    static size_t sz = 0;         /* number of cached terms */
    static size_t cap = 0;        /* allocated capacity */
    static idx_t curr = 1;        /* next integer to test */

    while (sz < n) {
        idx_t x = curr;
        unsigned cnt = 0;
        while (x > 0) {
            if ((x % 9) == 4) {
                ++cnt;
                if (cnt > 3) break;          /* early exit if too many 4's */
            }
            x /= 9;
        }
        if (cnt == 3) {
            if (sz == cap) {
                size_t new_cap = cap ? cap * 2 : 1024;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) return 0;         /* allocation failed – abort */
                cache = tmp;
                cap = new_cap;
            }
            cache[sz++] = (seq_t)curr;
        }
        ++curr;
    }
    return cache[n - 1];
}
