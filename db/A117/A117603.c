/*
 * A117603  Padovan numbers which can be divided by the product of their
 *          digits.
 * https://oeis.org/A117603
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 9, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Padovan numbers divisible by the product of their digits.
   Recurrence:  P(0)=P(1)=P(2)=1,  P(n)=P(n-2)+P(n-3)  for n≥3.
   a(n) contains the distinct values P(k) that satisfy
        P(k) % digit_product(P(k)) == 0.
   The values are generated in increasing order and cached so that
   any order of calls to a(idx_t n) is handled efficiently. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_product.h"

seq_t a(idx_t n)
{
    /* Static cache of already found terms */
    static seq_t *cache = NULL;
    static size_t cache_len = 0, cache_cap = 0;

    /* State for Padovan generation */
    static bool initialized = false;
    static seq_t a0 = 1, a1 = 1, a2 = 1; /* P(n-3), P(n-2), P(n-1) */
    static idx_t pad_idx = 0;            /* next Padovan index to process */

    /* Initialise cache and Padovan state on first call */
    if (!initialized) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        cache_len = 0;
        a0 = a1 = a2 = 1;
        pad_idx = 0;
        initialized = true;
    }

    /* Generate Padovan numbers until we have at least n qualifying terms */
    while (cache_len < n) {
        seq_t val;
        if (pad_idx < 3) {
            /* First three Padovan numbers are 1,1,1 */
            switch ((int)pad_idx) {
                case 0: val = a0; break;
                case 1: val = a1; break;
                default: val = a2; break;
            }
        } else {
            /* Next Padovan number = P(n-2)+P(n-3) = a0 + a1 */
            seq_t new_val = a0 + a1;

            /* Detect overflow for fixed‑size integer types:
               if new_val < a0, the addition wrapped around. */
            if (new_val < a0) {
                /* No further values can be represented accurately;
                   break out of the loop.  For arbitrary-precision
                   seq_t this test is always false, so execution
                   continues normally. */
                break;
            }

            val = new_val;
            /* Shift window for next iteration: P(n-2), P(n-1), P(n) */
            a0 = a1;
            a1 = a2;
            a2 = val;
        }

        /* Test the divisibility property */
        seq_t prod = digit_product(val);
        if (prod != 0 && (val % prod) == 0) {
            /* Store distinct values only */
            if (cache_len == 0 || val != cache[cache_len - 1]) {
                if (cache_len == cache_cap) {
                    cache_cap *= 2;
                    seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
                    if (!tmp) {
                        /* Allocation failure – terminate gracefully */
                        return 0;
                    }
                    cache = tmp;
                }
                cache[cache_len++] = val;
            }
        }

        pad_idx++; /* Advance to next Padovan index */
    }

    /* Return the requested term */
    return cache[n - 1];
}
