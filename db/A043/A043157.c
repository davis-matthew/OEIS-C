/*
 * A043157  Numbers k such that 0 and 6 occur juxtaposed in the base-8
 *          representation of k but not of k-1.
 * https://oeis.org/A043157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 7, u16 n <= 43, u32 n <= 43, u64 n <= 43, u128 n <= 43,
 *            big any n
 * Helpers:   has_adjacent0or6
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A043157: numbers k such that base‑8 digits 0 and 6 occur
   adjacent (in either order) in the base‑8 representation of k
   but not in that of k-1. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/has_adjacent0or6.h"

/* Buffer size for the cache – large enough for all
   n values that will be requested in the tests. */
#define CACHE_CAP 200000

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* undefined for offset 1, but kept safe */

    static seq_t cache[CACHE_CAP];
    static size_t cache_len = 0;
    static seq_t next_candidate = 1;  /* smallest integer to examine */

    if (n <= cache_len)            /* already computed */
        return cache[n - 1];

    /* Generate terms until we have at least n of them. */
    while (cache_len < n)
    {
        seq_t k = next_candidate++;
        if (has_adjacent0or6(k) && !has_adjacent0or6(k - 1))
        {
            cache[cache_len++] = k;
        }
    }
    return cache[n - 1];
}
