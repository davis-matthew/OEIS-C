/*
 * A044733  Numbers n such that string 2,0 occurs in the base 10
 *          representation of n but not of n+1.
 * https://oeis.org/A044733
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest integer m such that decimal string "20" appears
 *            in m but not in m+1. Computed by iterating integers and testing
 *            the property
 * Range:     u8 n <= 4, u16 n <= 39, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implementation relies only on integer arithmetic and standard
   C library functions.  It caches results so that repeated calls
   (in any order) are fast. */

#include <stdbool.h>
#include <stdlib.h>

static bool has20(seq_t x)
{
    /* Detect the substring "20" in the decimal representation of x.
       We scan from the least‑significant digit.  A pair (2,0) is found
       when the current digit is 2 and the previous (lower) digit is 0. */
    seq_t prev = 10;                     /* sentinel value */
    while (x > 0) {
        seq_t d = x % 10;
        if (prev != 10 && d == 2 && prev == 0)
            return true;
        prev = d;
        x /= 10;
    }
    return false;
}

seq_t a(idx_t n)
{
    /* Cached sequence of terms */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;
    static idx_t cache_len = 0;
    static seq_t last_checked = 0;      /* last number examined */

    if (n == 0)
        return 0;                       /* undefined for offset 1 */

    /* Grow cache until it contains at least n terms */
    while (cache_len < n) {
        /* Search for the next qualifying integer */
        for (seq_t m = last_checked + 1; ; ++m) {
            if (has20(m) && !has20(m + 1)) {
                /* Ensure capacity */
                if (cache_len >= cache_cap) {
                    size_t new_cap = cache_cap == 0 ? 8 : cache_cap * 2;
                    seq_t *tmp = (seq_t *)realloc(cache,
                                                   new_cap * sizeof(seq_t));
                    if (!tmp) return 0;     /* allocation failure */
                    cache = tmp;
                    cache_cap = new_cap;
                }
                cache[cache_len++] = m;
                last_checked = m;
                break;                   /* found next term */
            }
        }
    }

    return cache[n - 1];
}
