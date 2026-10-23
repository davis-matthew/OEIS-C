/*
 * A137001  Numbers k such that k and k^2 use only the digits 1, 2, 4 and 9.
 * https://oeis.org/A137001
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, more
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 6, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(n) is computed by searching for the n-th integer k such that
 * both k and k^2 consist only of the digits 1, 2, 4, and 9.
 * The search is performed incrementally and memoized, so calls
 * with arbitrary order of n are handled efficiently.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Helper: test whether all decimal digits of x are in {1,2,4,9}. */
static bool digits_ok(seq_t x)
{
    if (x == 0)
        return false;
    for (; x != 0; x /= 10) {
        unsigned d = (unsigned)(x % 10);
        if (d != 1 && d != 2 && d != 4 && d != 9)
            return false;
    }
    return true;
}

/* External function required by the test harness. */
seq_t a(idx_t n)
{
    /* Memoization storage */
    static seq_t *cache = NULL;   /* array of found terms */
    static size_t cache_len = 0;   /* number of stored terms */
    static idx_t next = 1;        /* next integer to test */

    /* Generate terms until we reach the desired index n */
    while (cache_len < n) {
        /* Test current candidate */
        if (digits_ok((seq_t)next)) {
            seq_t sq = (seq_t)next * (seq_t)next;
            if (digits_ok(sq)) {
                /* Append to cache */
                size_t new_len = cache_len + 1;
                seq_t *tmp;
                if (cache == NULL) {
                    tmp = (seq_t *)malloc(sizeof(seq_t));
                } else {
                    tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
                }
                if (tmp == NULL) {
                    /* Allocation failure: cannot proceed */
                    abort();
                }
                cache = tmp;
                cache[cache_len] = (seq_t)next;
                cache_len = new_len;
            }
        }
        /* Advance to next integer, handling overflow safely */
        if (++next == 0)
            break;  /* reached maximum of idx_t; should never happen in tests */
    }
    return cache[n - 1];
}
