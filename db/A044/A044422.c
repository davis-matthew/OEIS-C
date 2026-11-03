/*
 * A044422  Numbers n such that string 9,0 occurs in the base 10
 *          representation of n but not of n-1.
 * https://oeis.org/A044422
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Function computing a(n) for OEIS A044422: numbers n such that
   the decimal representation of n contains the substring "90"
   but the integer n-1 does not. The implementation searches
   successive integers and memoises found terms. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/mem.h"

/* Return true if the decimal representation of x contains the
   substring "90" (i.e., a digit '9' immediately followed by a
   digit '0'). */
static bool has90(seq_t x)
{
    seq_t t = x;
    while (t >= (seq_t)10) {
        if ((t % (seq_t)100) == (seq_t)90) return true;
        t /= (seq_t)10;
    }
    return false;
}

seq_t a(idx_t n)
{
    static seq_t *cache = NULL;   /* cached terms */
    static size_t cap   = 0;      /* allocated capacity */
    static size_t len   = 0;      /* number of elements stored */
    static idx_t cur    = 90;     /* next integer to test */

    if (n < 1) return (seq_t)0;   /* irrelevant for n<1 */

    /* Grow cache until we have at least n terms */
    while (len < (size_t)n) {
        /* Check if cur meets the criterion */
        if (has90((seq_t)cur) && !has90((seq_t)(cur - 1))) {
            if (len == cap) {
                size_t new_cap = cap ? cap * 2 : 16;
                seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                if (!tmp) abort();
                cache = tmp;
                cap   = new_cap;
            }
            cache[len++] = (seq_t)cur;
        }
        cur++;
    }

    return cache[n - 1];
}
