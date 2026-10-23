/*
 * A039451  Numbers whose base-11 representation has the same nonzero number
 *          of 1's and 5's.
 * https://oeis.org/A039451
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   numbers n whose base‑11 representation has the same non‑zero
 *            number of 1's and 5's. This function returns the n-th such
 *            number (OEIS A039451). It caches results so arbitrary call order
 *            is supported
 * Range:     u8 n <= 20, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;
    static size_t cap = 0;
    static size_t len = 0;
    static seq_t last_checked = 1; /* last number examined */

    /* ensure cache can hold n entries */
    if (len < (size_t)n) {
        size_t needed = (size_t)n;
        if (cap < needed) {
            size_t newcap = cap ? cap << 1 : 64;
            while (newcap < needed) newcap <<= 1;
            seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) exit(1);           /* allocation failure */
            cache = tmp;
            cap = newcap;
        }
        /* generate terms until we have n of them */
        while (len < needed) {
            last_checked++;              /* next integer to examine */
            seq_t val = last_checked;
            int c1 = 0, c5 = 0;
            while (val != (seq_t)0) {
                seq_t d = val % (seq_t)11;
                if (d == (seq_t)1) c1++;
                else if (d == (seq_t)5) c5++;
                val /= (seq_t)11;
            }
            if (c1 > 0 && c1 == c5) {
                cache[len++] = last_checked;
            }
        }
    }
    return cache[(size_t)n - 1];
}
