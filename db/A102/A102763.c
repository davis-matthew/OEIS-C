/*
 * A102763  a(1) = a(2) = 2, a(3) = 4; if n is even set a(n+1) = a([2n/5])+2,
 *          otherwise a(n+1) = a([3n/5]).
 * https://oeis.org/A102763
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: a(1)=2, a(2)=2, a(3)=4; for n≥3,
 *   a(n+1)= (n even) ? a(⌊2n/5⌋)+2 : a(⌊3n/5⌋)
 *
 * This implementation memoises a() using a dynamically‑resized array.
 * It works for built‑in unsigned integer types and for an arbitrary‑precision
 * integer class compiled with C++ (only standard operators are used).
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for memoised values */
    static seq_t *cache   = NULL;  /* array of computed terms  */
    static idx_t  cap     = 0;     /* allocated capacity          */
    static idx_t  size    = 0;     /* highest index computed (>=1)*/

    /* Initialise with the first three terms once */
    if (size == 0) {
        cap   = 4;
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        /* guard against allocation failure – unlikely for the used limits */
        if (!cache) abort();
        cache[0] = (seq_t)2; /* a(1) */
        cache[1] = (seq_t)2; /* a(2) */
        cache[2] = (seq_t)4; /* a(3) */
        size = 3;
    }

    /* Return already computed value */
    if (n <= size) return cache[n - 1];

    /* Grow the buffer if necessary */
    if (n > cap) {
        idx_t new_cap = cap ? cap << 1 : 4;
        while (new_cap < n) new_cap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute the missing terms up to n */
    for (idx_t i = size + 1; i <= n; ++i) {
        idx_t m = i - 1;  /* the “n” in the recurrence */

        idx_t j;          /* index used to look up a() */
        seq_t val;

        if ((m & 1) == 0) {          /* m is even → ⌊2m/5⌋ */
            j = (m * 2) / 5;
            val = cache[j - 1] + (seq_t)2;
        } else {                     /* m is odd  → ⌊3m/5⌋ */
            j = (m * 3) / 5;
            val = cache[j - 1];
        }
        cache[i - 1] = val;
    }
    size = n;
    return cache[n - 1];
}
