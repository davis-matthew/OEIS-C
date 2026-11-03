/*
 * A043256  Numbers k such that 6 and 8 occur juxtaposed in the base-10
 *          representation of k but not of k-1.
 * https://oeis.org/A043256
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 41 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Term definition: a(n) is the n-th positive integer k such that
   the decimal representation of k contains the sub‑string "68" or
   "86", and the decimal representation of k−1 does NOT contain that
   sub‑string.  The OEIS offset is 1. */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Check if a number contains the adjacent digits 68 or 86. */
static bool has_adj68or86(seq_t n)
{
    if (n < 10)          /* less than two digits – impossible */
        return false;

    seq_t prev = n % 10; /* ones digit */
    n /= 10;             /* remove ones digit */

    /* Process all subsequent digit pairs */
    while (n >= 10) {
        seq_t cur = n % 10;          /* current digit (tens place) */
        if ((cur == (seq_t)6 && prev == (seq_t)8) ||
            (cur == (seq_t)8 && prev == (seq_t)6))
            return true;
        prev = cur;
        n /= 10;
    }

    /* Remaining pair: the leading single digit (now in n) and prev */
    /* (n is now a single digit) */
    if ((n == (seq_t)6 && prev == (seq_t)8) ||
        (n == (seq_t)8 && prev == (seq_t)6))
        return true;

    return false;
}

seq_t a(idx_t n)          /* n ≥ 1 */
{
    /* Static cache of computed terms */
    static seq_t *cache     = NULL;
    static size_t cache_len = 0;
    static size_t cache_cap = 0;
    static seq_t next_k     = 1;   /* next integer to test */

    /* Ensure cache holds at least n terms */
    while (cache_len < (size_t)n) {
        /* Search for the next qualifying integer */
        while (1) {
            seq_t k = next_k++;
            if (has_adj68or86(k) && !has_adj68or86(k - (seq_t)1)) {
                /* Store k */
                if (cache_len == cache_cap) {
                    size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
                    seq_t *tmp = mem_resize(seq_t, cache, new_cap);
                    if (!tmp) { /* allocation failure – unwind to caller */
                        mem_free(cache);
                        cache = NULL;
                        cache_len = cache_cap = 0;
                        return (seq_t)0; /* will not happen in built tests */
                    }
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = k;
                break;
            }
        }
    }

    return cache[(size_t)n - 1];
}
