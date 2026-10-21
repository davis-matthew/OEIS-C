/*
 * A236207  Numbers not divisible by 5 or 11.
 * https://oeis.org/A236207
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 185, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Numbers not divisible by 5 or 11
    a(n) = the nth positive integer that is not a multiple of 5 or 11.
    This implementation uses a small cache so that repeated calls are fast
    and works for any n ≥ 1.  */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/grow_seq.h"   /* provides void grow_seq(seq_t **, size_t *, size_t); */

seq_t a(idx_t n)
{
    /* Static cache state */
    static seq_t *cache = NULL;
    static size_t cap   = 0;   /* allocated capacity */
    static idx_t count = 0;    /* number of cached terms */
    static idx_t next  = 1;    /* next integer to test for inclusion */
    static bool   done  = false;

    /* Ensure cache contains at least n terms */
    if (!done) { done = true; }
    if (count >= n) goto RETURN;

    if (cap < (size_t)n)            /* grow cache if necessary */
        grow_seq(&cache, &cap, (size_t)n);

    while (count < n) {
        if (next % 5U != 0 && next % 11U != 0) {
            cache[count] = (seq_t)next;
            count++;
        }
        next++;
    }

RETURN:
    return cache[n - 1];
}
