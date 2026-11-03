/*
 * A044548  Numbers n such that string 4,1 occurs in the base 7 representation
 *          of n but not of n+1.
 * https://oeis.org/A044548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = the n-th integer N such that the string "41" appears in
 *            the base‑7 representation of N but not in that of N+1. (OEIS
 *            A044548) The routine builds the sequence lazily and memoises all
 *            terms found so far
 * Range:     u8 n <= 6, u16 n <= 42, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Test whether a base‑7 representation of v contains the adjacent pair 4,1.
   We scan from least significant digit upwards; the pair "41" in normal
   left‑to‑right order appears as "1,4" in this reversed scan. */
static inline bool has_41(seq_t v)
{
    int prev = -1;             /* previous remainder after division by 7   */
    while (v > 0) {
        int r = (int)(v % 7);  /* remainder is always < 7               */
        if (prev == 1 && r == 4)
            return true;       /* "1,4" in reverse scan -> "4,1" found   */
        prev = r;
        v /= 7;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* static state: cache of found terms, current number to test and capacity */
    static seq_t *cache = NULL;   /* array of terms */
    static size_t cap = 0;        /* allocated capacity */
    static size_t sz  = 0;        /* number of terms stored */
    static seq_t cur  = 0;        /* last number processed (exclusive) */

    /* Grow the cache lazily */
    if (cap == 0) {
        cap = 16;
        cache = mem_new(seq_t, cap);
    }

    /* Generate terms until we have at least n of them */
    while (sz < n) {
        cur++; /* next candidate */
        if (has_41(cur) && !has_41(cur + 1)) {
            /* store it */
            if (sz + 1 > cap) {
                cap <<= 1; /* double the capacity */
                cache = mem_resize(seq_t, cache, cap);
            }
            cache[sz++] = cur;
        }
    }

    return cache[n - 1];
}
