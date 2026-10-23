/*
 * A039490  Numbers whose base-11 representation has the same nonzero number
 *          of 8's and 9's.
 * https://oeis.org/A039490
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth integer whose base-11 representation contains an
 *            equal non‑zero number of digits 8 and 9 (search)
 * Range:     u8 n <= 4, u16 n <= 44, u32 n <= 44, u64 n <= 44, u128 n <= 44,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* offset starts at 1, but guard against 0 */

    static seq_t *cache = NULL;               /* memoised sequence values */
    static size_t cap   = 0;
    static size_t len   = 0;
    static idx_t  current = 1;                /* next integer to test */

    /* Generate terms until we have at least n entries */
    while (len < n) {
        idx_t x      = current;
        unsigned count8 = 0, count9 = 0;

        /* Count digits 8 and 9 in the base‑11 representation */
        while (x > 0) {
            unsigned d = (unsigned)(x % 11);
            if (d == 8) ++count8;
            else if (d == 9) ++count9;
            x /= 11;
        }

        /* Valid term if both counts are positive and equal */
        if (count8 > 0 && count8 == count9) {
            /* Grow cache if necessary */
            if (len == cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();         /* allocation failure */
                cache = tmp;
                cap   = new_cap;
            }
            cache[len++] = (seq_t)current;
        }
        ++current;                           /* next integer to examine */
    }

    return cache[n - 1];                      /* offset 1: first term is cache[0] */
}
