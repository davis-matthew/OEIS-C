/*
 * A062909  Non-palindromic number and its reversal are both multiples of 4.
 * https://oeis.org/A062909
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   This function generates the nth non‑palindromic multiple of 4 *
 *            for which its decimal reversal is also a multiple of 4. The
 *            sequence is * searched incrementally by testing successive
 *            multiples of 4
 * Range:     u8 n <= 6, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   ensure_capacity, is_palindrome, reverse_decimal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_palindrome.h"
#include "../utils/reverse_decimal.h"
#include "../utils/ensure_capacity.h"

seq_t a(idx_t n)
{
    if (n == 0)                      /* behaviour below the offset is irrelevant */
        return (seq_t)0;

    /* Static cache of already generated terms. */
    static seq_t *cache      = NULL;
    static size_t cache_len  = 0;    /* number of terms stored   */
    static size_t cache_cap  = 0;    /* allocated capacity       */
    static seq_t last_checked = 0;   /* largest number examined   */

    /* Generate terms until we have at least n of them. */
    while (cache_len < n) {
        seq_t cand = last_checked + 4;     /* next candidate (multiple of 4) */

        /* Scan forward for the next valid term. */
        for (;;) {
            /* Safety: insure the first candidate is at least 4. */
            if (cand < 4) cand = 4;

            /* Reject palindromic candidates. */
            if (is_palindrome(cand, 10)) {
                cand += 4;
                continue;
            }

            /* The reversal must also be a multiple of 4. */
            seq_t rev = reverse_decimal(cand);
            if ((rev % 4) == 0) {
                /* Append the valid term to the cache. */
                ensure_capacity(cache_len + 1, &cache_cap,
                                (void **)&cache, sizeof(seq_t));
                cache[cache_len++] = cand;
                last_checked = cand;   /* next search starts after this value */
                break;                 /* outer while loop will check if more needed */
            }

            cand += 4;                 /* try the next multiple of 4 */
        }
    }

    return cache[n - 1];
}
