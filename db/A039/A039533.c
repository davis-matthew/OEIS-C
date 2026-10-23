/*
 * A039533  Numbers whose base-12 representation has the same nonzero number
 *          of 4's and 7's.
 * https://oeis.org/A039533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = nth integer whose base‑12 representation has the * same
 *            (and non‑zero) number of digits 4 and 7. The implementation *
 *            generates terms lazily, caching already found terms so that
 *            subsequent * calls are efficient regardless of call order
 * Range:     u8 n <= 4, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* array holding found terms          */
    static idx_t cached = 0;      /* number of terms stored             */
    static idx_t cap = 0;         /* capacity of the cache              */
    static seq_t next = 1;        /* next integer to test               */

    if (n == 0) return 0;         /* safety – OEIS offset is 1          */

    /* Produce new terms until we have at least n of them. */
    for (; cached < n; ++next) {
        seq_t x = next;
        unsigned count4 = 0, count7 = 0;

        /* Count 4's and 7's in the base‑12 representation of x. */
        while (x != 0) {
            seq_t d = x % (seq_t)12;
            if (d == 4) ++count4;
            else if (d == 7) ++count7;
            x = x / (seq_t)12;
        }

        if (count4 == count7 && count4 > 0) {
            /* Ensure the cache has room for a new term. */
            if (cached == cap) {
                size_t new_cap = (cap == 0) ? 16 : (size_t)cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) { abort(); }          /* allocation failure */
                cache = tmp;
                cap = (idx_t)new_cap;
            }
            cache[cached++] = next;
        }
    }

    return cache[n - 1];
}
