/*
 * A044074  Numbers k such that string 3,1 occurs in the base 4 representation
 *          of k but not of k-1.
 * https://oeis.org/A044074
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 20, u16 n <= 3976, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   has_substring_31
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the n-th positive integer k such that the base‑4 representation of
 * k contains the substring "31" while the base‑4 representation of k‑1 does
 * not.  The function keeps a static cache of previously found terms so it
 * works correctly regardless of the order of calls.  The cache size is
 * chosen to comfortably cover the required range (n ≤ ~10 000).  The code
 * is plain C11 and also compiles as C++17. */

#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>
#include "../utils/has_substring_31.h"

/* Maximum number of terms the static cache can hold.  This comfortably
 * exceeds the number of terms that will be requested in typical tests. */
#define MAX_CACHE 200000

static seq_t cache[MAX_CACHE];
static idx_t cache_len = 0;   /* number of terms already stored */
static seq_t next_k = 1;      /* next integer to test for membership */

/* External linkage as required */
seq_t a(idx_t n)
{
    if (n == 0)   /* undefined below the offset; return 0 as a safe default */
        return (seq_t)0;

    /* Compute terms until we have reached index n */
    while (cache_len < n) {
        /* Ensure we do not overflow the static cache array */
        if (cache_len >= MAX_CACHE) {
            /* In an unlikely case where the cache is too small, fall back
             * to returning the value without caching it. */
            while (1) {
                if (has_substring_31(next_k) && !has_substring_31(next_k - ((seq_t)1))) {
                    return next_k;
                }
                ++next_k;
            }
        }

        /* Find the next qualifying integer */
        while (1) {
            if (has_substring_31(next_k) && !has_substring_31(next_k - ((seq_t)1))) {
                cache[cache_len++] = next_k;
                ++next_k;
                break;
            }
            ++next_k;
        }
    }

    return cache[n - 1];
}
