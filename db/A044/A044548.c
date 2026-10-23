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
 * Helpers:   has_41
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_41.h"

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
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
    }

    /* Generate terms until we have at least n of them */
    while (sz < n) {
        cur++; /* next candidate */
        if (has_41(cur) && !has_41(cur + 1)) {
            /* store it */
            if (sz + 1 > cap) {
                cap <<= 1; /* double the capacity */
                cache = (seq_t *)realloc(cache, cap * sizeof(seq_t));
            }
            cache[sz++] = cur;
        }
    }

    return cache[n - 1];
}
