/*
 * A041516  Numerators of continued fraction convergents to sqrt(275).
 * https://oeis.org/A041516
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, frac, easy
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 41, u128 n <= 88,
 *            big any n
 * Helpers:   seq_alloc, seq_realloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A041516: Numerators of continued fraction convergents to sqrt(275)
   Formula: a(0)=16; for n≥1,
   a(n)=period[(n-1)%6]*a(n-1)+a(n-2)
   where period = {1,1,2,1,1,32}.  This recurrence follows from the
   continued‑fraction expansion [16;(1,1,2,1,1,32)] repeating.  */

/* The function maintains a memoized cache so that calls in any order are fast. */
#include <stddef.h>
#include <stdint.h>
#include "../utils/seq_alloc.h"
#include "../utils/seq_realloc.h"

seq_t a(idx_t n)
{
    /* Static cache for computed values */
    static seq_t *cache = NULL;
    static size_t len = 0;     /* number of elements stored (>=1 if cache exists) */
    static size_t cap = 0;     /* allocated capacity */

    /* Period of the continued‑fraction coefficients after the initial term */
    static const int period[6] = {1, 1, 2, 1, 1, 32};

    /* Initial allocation and first value */
    if (cap == 0) {
        cap = 64;                       /* arbitrary initial capacity */
        cache = seq_alloc(cap);
        cache[0] = (seq_t)16;           /* a(0) = 16 */
        len   = 1;
    }

    /* Extend cache if needed */
    if (len <= (size_t)n) {
        size_t new_cap = cap;
        while (new_cap <= (size_t)n) new_cap <<= 1;  /* double until sufficient */

        seq_t *tmp = seq_realloc(cache, cap, new_cap);
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute missing values up to n */
    if (len <= (size_t)n) {
        /* Prepare prior two terms for recurrence */
        seq_t prev, prevprev;
        if (len == 1) {                /* only a(0) known */
            prev     = cache[0];      /* a(0) */
            prevprev = (seq_t)1;      /* conceptual a(-1)=1 */
        } else {                       /* len >= 2 */
            prev     = cache[len-1];
            prevprev = cache[len-2];
        }

        /* Iterate from current length up to n */
        for (size_t k = len; k <= (size_t)n; ++k) {
            int ak = period[(k-1) % 6];          /* coefficient a_k */
            seq_t curr = (seq_t)ak * prev + prevprev;
            cache[k] = curr;
            /* shift for next iteration */
            prevprev = prev;
            prev     = curr;
        }
        len = (size_t)n + 1;
    }

    return cache[(size_t)n];
}
