/*
 * A044068  Numbers n such that string 1,3 occurs in the base 4 representation
 *          of n but not of n-1.
 * https://oeis.org/A044068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 48, u32 n <= 48, u64 n <= 48, u128 n <= 48,
 *            big any n
 * Helpers:   maybe_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044068: numbers n such that the base‑4 representation of n contains
   the substring "13" (digit 1 followed by digit 3) but the base‑4 representation
   of n‑1 does not.  The function computes terms on demand, storing found
   values in a static cache to support out‑of‑order calls. */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/maybe_grow.h"

/* check whether x in base‑4 has the substring 1,3 */
static bool has13(seq_t x)
{
    if (x == 0)
        return false;

    int prev = -1;                 /* previous (less significant) digit */
    while (x > 0) {
        unsigned digit = (unsigned)(x % 4);
        if (digit == 1 && prev == 3)
            return true;
        prev = digit;
        x /= 4;
    }
    return false;
}

/* retrieve the n‑th term of A044068 (n≥1) */
seq_t a(idx_t n)
{
    static seq_t *cache   = NULL;   /* cached terms */
    static size_t sz     = 0;       /* number of cached terms */
    static size_t cap    = 0;       /* current capacity */
    static seq_t next    = 1;       /* next integer to examine */

    /* ensure the cache can hold at least n terms */
    while (sz < (size_t)n) {
        /* find the next qualifying integer */
        for (;;) {
            if (has13(next) && !has13(next - 1)) {
                maybe_grow(&cache, &sz, &cap);
                cache[sz++] = next;
                next++;                     /* start looking after this value */
                break;
            }
            next++;
        }
    }
    return cache[(size_t)n - 1];
}
