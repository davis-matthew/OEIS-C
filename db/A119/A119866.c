/*
 * A119866  Numbers whose square starts with 5 identical digits.
 * https://oeis.org/A119866
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 none, u32 n <= 4, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   digit_len, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Find the n‑th integer whose square starts with exactly five identical
   decimal digits.  The sequence is a(1)=10541, a(2)=33334, …   (OEIS A119866) */

#include <stdlib.h>
#include "../utils/digit_len.h"
#include "../utils/pow10.h"

/* Return the n‑th term of A119866 (offset 1).  The function is
   memoised internally, so it works for calls in any order. */
seq_t a(idx_t n)
{
    /* Static cache of found terms.  The cache grows on demand. */
    static seq_t *cache = NULL;  /* array of terms that have been found   */
    static size_t cap  = 0;      /* allocated capacity of cache           */
    static size_t len  = 0;      /* number of terms currently stored      */

    /* Current candidate integer and its square (updated incrementally). */
    static seq_t x = 1;          /* candidate number                      */
    static seq_t y = 1;          /* squared value of x                    */

    /* The sequence is defined for n >= 1; a(0) is unused. */
    if (n == 0) return 0;

    /* Expand the search until we have found at least n terms. */
    while (len < n) {
        /* y holds x².  Check whether the first five digits of y are equal. */
        int d = digit_len(y);            /* number of decimal digits of y     */
        if (d >= 5) {
            /* Compute 10^(d‑5) to truncate the last d‑5 digits. */
            seq_t p10   = pow10((idx_t)(d - 5));
            seq_t first = y / p10;                 /* leading 5 digits of y           */
            seq_t t     = first / 11111;           /* candidate leading digit          */
            /* Verify that first == t * 11111 and that t is a single digit 1‑9. */
            if (first == t * 11111 && t >= 1 && t <= 9) {
                /* Store the term if the property holds. */
                if (len >= cap) {
                    /* Grow the cache: double capacity or set to 4 if zero. */
                    size_t new_cap = cap ? cap * 2 : 4;
                    while (new_cap <= len) new_cap <<= 1;
                    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                    if (!tmp) exit(EXIT_FAILURE);
                    cache = tmp;
                    cap   = new_cap;
                }
                cache[len++] = x;
            }
        }

        /* Advance to the next integer: compute (x+1)² from x². */
        seq_t old = x;
        x = old + 1;
        y = y + 2 * old + 1;   /* (x+1)² = x² + 2·x + 1 */
    }

    /* Return the cached term.  n ≥ 1 guarantees n‑1 < len. */
    return cache[n - 1];
}
