/*
 * A044699  Numbers n such that string 7,4 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044699
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Numbers n such that string "7,4" occurs in the base‑9 representation of n
   but not in that of n+1.  The function returns a( n ) with OEIS offset 1. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

/* Return true if 'n' written in base 9 contains the substring 7,4. */
static bool has_substring_74(seq_t n)
{
    bool prev4 = false;
    while (n != 0) {
        uint8_t d = (uint8_t)(n % 9);
        n /= 9;
        if (prev4 && d == 7)
            return true;
        prev4 = (d == 4);
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Static cache: cache[i] is a(i+1). */
    static seq_t *cache = NULL;
    static idx_t cached = 0;
    static size_t cap = 0;
    static seq_t last = 0;   /* last value tested */

    while (cached < n) {
        /* Find next number satisfying the condition. */
        do {
            last++;
        } while (!(has_substring_74(last) && !has_substring_74(last + 1)));

        /* Grow the cache if necessary. */
        if (cached == cap) {
            size_t newcap = (cap == 0) ? 16 : cap * 2;
            if (newcap < cap + 1) newcap = cap + 1;
            seq_t *tmp = mem_resize(seq_t, cache, newcap);
            if (!tmp) exit(EXIT_FAILURE);  /* allocation failure */
            cache = tmp;
            cap = newcap;
        }

        cache[cached++] = last;
    }

    return cache[n - 1];
}
