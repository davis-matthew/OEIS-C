/*
 * A044695  Numbers n such that string 7,0 occurs in the base 9 representation
 *          of n but not of n+1.
 * https://oeis.org/A044695
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

/* Computed by brute force search. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: check whether a base‑9 number x contains the substring "70" */
static bool has70(seq_t x)
{
    if (x == 0) return false;            /* "0" never contains "70" */

    seq_t t = x;
    bool prev_is0 = false;                /* was the previous digit 0? */

    while (t > 0) {
        seq_t mod = t % 9;
        bool cur_is7 = (mod == 7);
        if (prev_is0 && cur_is7) return true;
        prev_is0 = (mod == 0);
        t = t / 9;
    }
    return false;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* static cache for previously found terms */
    static seq_t *cache    = NULL;
    static idx_t   capacity = 0;
    static idx_t   count    = 0;   /* number of terms stored          */
    static seq_t   next     = 1;   /* next integer to test            */

    /* grow cache if necessary */
    if (count == capacity) {
        size_t new_cap = capacity == 0 ? 16 : capacity * 2;
        seq_t *tmp = mem_resize(seq_t, cache, new_cap);
        if (!tmp) return (seq_t)0;          /* allocation failed */
        cache = tmp;
        capacity = new_cap;
    }

    /* generate terms until we reach the nth one */
    while (count < n) {
        seq_t val = next;
        if (has70(val) && !has70(val + 1)) {
            cache[count] = val;
            ++count;
        }
        next = next + 1;
    }

    return cache[n - 1];
}
