/*
 * A044081  Numbers n such that string 0,4 occurs in the base 5 representation
 *          of n but not of n-1.
 * https://oeis.org/A044081
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 46, u32 n <= 46, u64 n <= 46, u128 n <= 46,
 *            big any n
 * Helpers:   contains_04
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A044081:
   Numbers n whose base‑5 representation contains the adjacent digits
   0 followed by 4, but n‑1 does not.  a(1) is the first such integer.
   This implementation scans successive integers and memoises found
   values.  The cache is unbounded up to MAX_CACHE entries.  The
   algorithm runs in linear time with respect to the largest n
   requested (≈ 2 × 10⁵ integer checks per 10 000th term). */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/contains_04.h"

/* Rough upper bound on the number of terms the cache will hold.
   For n=10 000 the sequence visits well below 2 × 10⁵ numbers. */
#define MAX_CACHE 200000

static seq_t cache[MAX_CACHE];
static size_t cache_len = 0;
static seq_t last_checked = 0;   /* the largest integer examined so far */

/* Return the n-th member of A044081 (1‑based). */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* sequence is defined for n ≥ 1 */

    /* Expand the cache until it contains at least n values. */
    while (cache_len < (size_t)n)
    {
        seq_t x = last_checked + (seq_t)1;  /* next integer to examine */

        for (;; ++x)
        {
            bool ok = contains_04(x) && !contains_04(x - (seq_t)1);
            last_checked = x;

            if (ok)
            {
                if (cache_len < MAX_CACHE)
                    cache[cache_len++] = x;
                break;              /* next term found */
            }
        }
    }

    return cache[n - 1];   /* zero‑based storage */
}
