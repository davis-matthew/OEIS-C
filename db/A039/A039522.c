/*
 * A039522  Numbers whose base-12 representation has the same nonzero number
 *          of 2's and 11's.
 * https://oeis.org/A039522
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = the n-th positive integer whose base‑12 representation
 *            has the same non‑zero number of digits 2 and 11
 * Range:     u8 n <= 3, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;
    static size_t cap   = 0;
    static size_t len   = 0;
    static idx_t  checked = 0;          /* last examined integer */

    const size_t target = (size_t)n;    /* 1‑based target index */

    /* initialise cache */
    if (cache == NULL) {
        cap = 8;
        cache = (seq_t*)malloc(cap * sizeof(seq_t));
        len = 0;
    }

    while (len < target) {
        ++checked;                      /* examine next integer */

        /* count occurrences of digits 2 and 11 in base 12 */
        idx_t x = checked;
        unsigned cnt2 = 0, cnt11 = 0;
        while (x > 0) {
            unsigned d = (unsigned)(x % (idx_t)12);
            if (d == 2)
                ++cnt2;
            else if (d == 11)
                ++cnt11;
            x /= 12;
        }

        if (cnt2 == cnt11 && cnt2 > 0) {
            if (len == cap) {           /* grow cache */
                cap <<= 1;
                seq_t *tmp = (seq_t*)realloc(cache, cap * sizeof(seq_t));
                if (tmp)
                    cache = tmp;
            }
            cache[len++] = (seq_t)checked;
        }
    }

    return cache[target - 1];
}
