/*
 * A039566  Numbers whose base-5 representation has the same number of 0's,
 *          2's and 3's.
 * https://oeis.org/A039566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 32, u16 n <= 55, u32 n <= 55, u64 n <= 55, u128 n <= 55,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: recursively build numbers whose base‑5 representation has
   equal counts of the digits 0, 2, and 3.  The function evaluates the
   terms by scanning natural numbers, counting the relevant digits, and
   caching the results so that repeated calls in any order remain
   efficient. */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n) {
    static seq_t *cache = NULL;     /* stored terms */
    static size_t cap   = 0;        /* allocated capacity of cache */
    static idx_t next   = 1;        /* next integer to examine */
    static idx_t seen   = 0;        /* number of terms stored in cache */

    /* ensure cache holds at least n terms */
    while (seen < n) {
        /* compute counts of 0,2,3 in base‑5 representation of next */
        idx_t x = next++;
        unsigned c0 = 0, c2 = 0, c3 = 0;
        if (x == 0) { /* never reached since we start from 1 */
            c0 = c2 = c3 = 0;
        } else {
            while (x > 0) {
                unsigned digit = x % 5;
                x /= 5;
                if (digit == 0) ++c0;
                else if (digit == 2) ++c2;
                else if (digit == 3) ++c3;
            }
        }
        if (c0 == c2 && c2 == c3) {
            /* store term if cache capacity exceeded */
            if (seen == cap) {
                size_t new_cap = cap ? cap * 2 : 128;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort(); /* memory failure */
                cache = tmp;
                cap = new_cap;
            }
            cache[seen++] = (seq_t)(next - 1);
        }
    }
    return cache[n - 1];
}
