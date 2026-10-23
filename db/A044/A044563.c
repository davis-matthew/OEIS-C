/*
 * A044563  Numbers n such that string 6,2 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Helpers:   has62
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes OEIS sequence A044563:
   Numbers n such that the base‑7 representation of n contains the
   substring “62” while the base‑7 representation of n+1 does not.  */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has62.h"

seq_t a(idx_t n)
{
    /* Static cache of the sequence values.  */
    static seq_t *cache = NULL;  /* array of values */
    static idx_t count = 0;      /* number of cached elements */
    static size_t cap = 0;       /* current capacity of cache */
    static seq_t last = 0;       /* last number for which the condition was true */

    /* Ensure the cache contains at least n elements.  */
    while (count < n) {
        /* Search for the next number satisfying the condition.  */
        seq_t cand = last + 1;
        while (1) {
            if (has62(cand) && !has62(cand + 1)) {
                /* Grow the cache if necessary.  */
                if (count >= cap) {
                    size_t new_cap = (cap == 0 ? 16 : cap * 2);
                    seq_t *new_cache = (seq_t *)realloc(cache,
                                                        new_cap * sizeof(seq_t));
                    cache = new_cache;
                    cap = new_cap;
                }
                cache[count++] = cand;
                last = cand;
                break;      /* found the next element */
            }
            cand++;       /* try next candidate */
        }
    }

    return cache[n - 1];
}
