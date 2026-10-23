/*
 * A039246  Numbers whose base-12 representation has the same number of 3's
 *          and 11's.
 * https://oeis.org/A039246
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 70, u16 n <= 70, u32 n <= 70, u64 n <= 70, u128 n <= 70,
 *            big any n
 * Helpers:   has_equal_3_11
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: scan integers in increasing order, selecting those whose
   base‑12 representation contains an equal number of digit 3’s and digit
   11’s.  The result is memoised in a static array so that arbitrary
   ordering of calls is handled efficiently. */
#include <stddef.h>
#include <stdbool.h>
#include "../utils/has_equal_3_11.h"

/* Adjust the following size if larger values of n are required. */
#define MAX_CACHE 200000        /* maximum number of terms we will store */

/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* Static cache for memoisation (thread‑unsafe). */
    static seq_t cache[MAX_CACHE];
    static size_t cache_len = 0;   /* number of terms stored so far */
    static idx_t  next_scan = 0;   /* next integer to test */

    /* If we have already computed the requested term, return it. */
    if (n <= cache_len)
        return cache[n - 1];

    /* Generate further terms until index n is reached. */
    while (cache_len < n) {
        if (has_equal_3_11(next_scan))
            cache[cache_len++] = (seq_t)next_scan;
        ++next_scan;
    }

    return cache[n - 1];
}
