/*
 * A021342  Decimal expansion of 1/338.
 * https://oeis.org/A021342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cons, easy
 * Formula:   a(n) = the n‑th digit after the decimal point in the expansion
 *            of 1/338 (OEIS A021342). The function generates the digits by
 *            repeated long division, caching the results so that
 *            non‑monotonic calls are handled in amortised O(1) time. The
 *            cache stores the digits as 8‑bit values; the returned value...
 * Range:     u8 n <= 98, u16 n <= 98, u32 n <= 98, u64 n <= 98, u128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* cache of digits 0‑9, stored as unsigned bytes to avoid type issues */
    static uint8_t *cache = NULL;
    static size_t   cap   = 0;          /* allocated size of cache          */
    static idx_t    last  = 0;          /* last index that has been computed */
    static idx_t    rem   = 1;          /* remainder after computing up to 'last' */
    static bool     init  = false;      /* flag to initialise the state     */

    idx_t start;   /* index at which to start computing new digits */

    /* Initialise state on first call */
    if (!init) {
        init   = true;
        start  = 0;
        rem    = 1;
        last   = 0;  /* will be updated after the first loop   */
    } else {
        /* If the requested index has already been computed, return it */
        if (n <= last) {
            return (seq_t)cache[n];
        }
        /* Otherwise compute forward from the last computed index */
        start = last + 1;
    }

    /* Ensure the cache is large enough to hold up to index n */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap ? (cap * 2) : 1;
        while (new_cap <= (size_t)n) {
            new_cap *= 2;
        }
        /* realloc works for any C++/C compiler; cast for C++ */
        uint8_t *tmp = (uint8_t *)realloc(cache, new_cap * sizeof(uint8_t));
        if (tmp == NULL) {
            /* Allocation failure – return 0 as a safe fallback */
            return 0;
        }
        cache = tmp;
        cap   = new_cap;
    }

    /* Perform long division steps from 'start' to 'n' inclusive */
    for (idx_t i = start; i <= n; ++i) {
        rem = rem * 10;                /* shift left by a decimal place   */
        uint8_t digit = (uint8_t)(rem / 338);  /* next decimal digit */
        rem = rem % 338;               /* new remainder                   */
        cache[i] = digit;              /* store the digit                 */
    }
    last = n;  /* update the last computed index */

    return (seq_t)cache[n];
}
