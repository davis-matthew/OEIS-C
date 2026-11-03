/*
 * A044809  Numbers k such that string 9,6 occurs in the base 10
 *          representation of k but not of k+1.
 * https://oeis.org/A044809
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Validated: partial, pass, 39 terms through n = 39 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A044809:  numbers k such that the decimal representation
 *  of k contains the substring "96" but the representation of k+1
 *  does not.  This implementation scans integers, tests the
 *  condition, and memoises the results in a static array so that
 *  later calls reuse previously computed terms.  The algorithm is
 *  O(n) in the index, with a small constant factor.  */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* check whether x (non‑negative) contains the decimal substring 96 */
static bool has96(seq_t x)
{
    if (x < 10)                 /* a single digit cannot contain "96" */
        return false;
    seq_t prev = x % 10;        /* least significant digit */
    x /= 10;
    while (x != 0) {
        seq_t cur = x % 10;     /* next digit to the left */
        if (cur == 9 && prev == 6)   /* forward order "96" (9 then 6) */
            return true;
        prev = cur;
        x /= 10;
    }
    return false;
}

/* sequence generator for A044809 */
seq_t a(idx_t n)
{
    /* static cache of previously found terms */
    static seq_t *cache = NULL;
    static size_t cache_cap = 0;   /* allocated elements */
    static size_t cache_len = 0;   /* number of terms stored */
    static seq_t next_k = 96;      /* next integer to test */

    if (n == 0)                 /* undefined in OEIS, but return 0 */
        return 0;

    /* extend cache until it contains the n-th term */
    while (cache_len < (size_t)n) {
        if (has96(next_k) && !has96(next_k + 1)) {
            /* grow cache if needed */
            if (cache_len == cache_cap) {
                cache_cap = cache_cap ? cache_cap * 2 : 64;
                cache = mem_resize(seq_t, cache, cache_cap);
            }
            cache[cache_len++] = next_k;
        }
        next_k++;
    }

    return cache[(size_t)n - 1];
}
