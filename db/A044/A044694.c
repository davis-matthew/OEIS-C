/*
 * A044694  Numbers n such that string 6,8 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044694
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   has68_base9
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that the base‑9 representation of n contains
 * the substring "68" but the base‑9 representation of n+1 does not. */

/* The function a(n) returns the nth term of this sequence.  */
/* It searches integers incrementally, storing found terms in a static
 * buffer so that subsequent calls are efficient, regardless of the order
 * of arguments. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has68_base9.h"

seq_t a(idx_t n)
{
    /* Static state: cache of found terms and last searched value. */
    static seq_t *cache = NULL;   /* array of found terms        */
    static size_t cap = 0;        /* allocated capacity          */
    static idx_t count = 0;       /* number of terms stored      */
    static seq_t last_checked = 0;/* value of last n examined    */

    /* Expand the cache as needed. */
    if (cache == NULL) {
        cap = 16;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) exit(1);
    }

    /* Find terms until we reach the requested index. */
    while (count < n) {
        last_checked++;          /* examine next integer */

        /* Test the condition. */
        if (has68_base9(last_checked) && !has68_base9(last_checked + 1)) {
            if ((size_t)count + 1 > cap) {
                size_t newcap = cap * 2;
                seq_t *newp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
                if (!newp) exit(1);
                cache = newp;
                cap = newcap;
            }
            cache[(size_t)count++] = last_checked;
        }
    }

    return cache[(size_t)(n - 1)];
}
