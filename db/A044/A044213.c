/*
 * A044213  Numbers n such that string 3,2 occurs in the base 8 representation
 *          of n but not of n-1.
 * https://oeis.org/A044213
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) is the nth positive integer whose base‑8 representation
 *            contains the substring "32", while (n-1) does not contain "32"
 * Range:     u8 n <= 5, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

static bool has32(seq_t x)
{
    /* Return true if the base‑8 representation of x contains the
       consecutive digits 3 followed by 2 (i.e. substring "32").  */
    seq_t prev = x % (seq_t)8;
    x = x / (seq_t)8;
    while (x > (seq_t)0) {
        seq_t curr = x % (seq_t)8;
        if (curr == (seq_t)3 && prev == (seq_t)2)
            return true;
        prev = curr;
        x = x / (seq_t)8;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Cache of already found terms for fast repeated queries. Dynamic
       storage is used; growth factor of 2.  */
    static seq_t *cache = NULL;
    static size_t sz = 0;
    static size_t cap = 0;
    static seq_t last_checked = (seq_t)0;   /* last integer examined */

    /* Ensure cache has at least n elements.  */
    while (sz < n) {
        /* examine next integer */
        last_checked = last_checked + (seq_t)1;

        /* test condition for the current integer */
        if (!has32(last_checked - (seq_t)1) && has32(last_checked)) {
            /* extend cache if necessary */
            if (sz >= cap) {
                size_t new_cap = (cap == 0) ? 8 : cap * 2;
                seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                if (!tmp) abort();        /* out of memory */
                cache = tmp;
                cap = new_cap;
            }
            cache[sz++] = last_checked;
        }
    }

    return cache[n - 1];
}
