/*
 * A044880  In base 9, (sum of even run lengths)=(sum of odd run lengths).
 * https://oeis.org/A044880
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Validated: partial, pass, 45 terms through n = 45 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for integers whose base‑9 run‑length sums are equal. */
#include <stddef.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/mem.h"

/* Helper: test if the base‑9 expansion of v satisfies the property. */
static bool good_base9_run_property(seq_t v)
{
    seq_t oddSum  = 0;
    seq_t evenSum = 0;
    seq_t prev    = (seq_t)-1;          /* sentinel */
    seq_t runLen  = 0;
    seq_t tmp     = v;

    while (tmp > 0) {
        seq_t digit = tmp % (seq_t)9;
        tmp /= (seq_t)9;
        if (digit == prev) {
            runLen++;
        } else {
            if (runLen > 0) {
                if ((runLen & (seq_t)1) == 0)
                    evenSum += runLen;
                else
                    oddSum += runLen;
            }
            prev   = digit;
            runLen = 1;
        }
    }
    /* handle the final run */
    if (runLen > 0) {
        if ((runLen & (seq_t)1) == 0)
            evenSum += runLen;
        else
            oddSum += runLen;
    }
    return oddSum == evenSum;
}

/* Main sequence function.  O(n) incremental generation with memoisation. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* undefined, but harmless */

    /* Static cache for previously computed terms */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;   /* number of terms stored */
    static size_t cache_cap  = 0;   /* allocated capacity */
    static seq_t next_val    = 1;   /* next integer to test */

    while (cache_len < n) {
        /* Ensure enough capacity for a new term */
        if (cache_len == cache_cap) {
            size_t new_cap = cache_cap ? cache_cap * 2 : 16;
            seq_t *tmp = mem_resize(seq_t, cache, new_cap);
            if (!tmp) return 0;            /* allocation failed */
            cache = tmp;
            cache_cap = new_cap;
        }
        /* Find the next number that satisfies the property */
        while (1) {
            if (good_base9_run_property(next_val)) {
                cache[cache_len++] = next_val;
                next_val++;
                break;
            }
            next_val++;
        }
    }
    return cache[n - 1];
}
