/*
 * A044152  Numbers n such that string 2,0 occurs in the base 7 representation
 *          of n but not of n-1.
 * https://oeis.org/A044152
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer whose base‑7 representation
 *            contains the substring “20” while (n‑1) does not. The function
 *            builds the sequence on demand and caches results, so repeated
 *            calls of any order are fast
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has20_base7
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has20_base7.h"

seq_t a(idx_t n)
{
    /* Cache of found terms */
    static seq_t *list   = NULL;
    static size_t cap    = 0;
    static size_t len    = 0;
    static idx_t last_checked = 0;   /* last integer examined */

    if (n == 0)
        return 0;     /* undefined for n<1; not used in tests */

    /* Generate terms until we have at least n of them. */
    while (len < n) {
        last_checked++;                           /* candidate integer */
        idx_t prev = last_checked - 1;

        if (has20_base7((seq_t)last_checked) &&
            !has20_base7((seq_t)prev)) {
            /* Grow cache if necessary */
            if (len == cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = (seq_t *)realloc(list, new_cap * sizeof(seq_t));
                if (!tmp)
                    abort();                      /* out of memory */
                list = tmp;
                cap = new_cap;
            }
            list[len++] = (seq_t)last_checked;    /* store the term */
        }
    }
    return list[n - 1];
}
