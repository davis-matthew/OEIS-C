/*
 * A017851  Expansion of 1/(1-x^6-x^7-x^8-x^9-x^10-x^11).
 * https://oeis.org/A017851
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 34, u16 n <= 60, u32 n <= 110, u64 n <= 212,
 *            u128 n <= 414, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A017851
 *  a(n) = a(n-6) + a(n-7) + a(n-8) + a(n-9) + a(n-10) + a(n-11)  (n > 10)
 *  a(0) = 1,  a(1)=a(2)=a(3)=a(4)=a(5) = 0
 *  a(6)=a(7)=a(8)=a(9)=a(10)=a(11) = 1
 *
 *  The routine keeps a static cache of computed values, so repeated
 *  calls are O(1) after the first cache build.  The cache grows as
 *  needed; initialisation happens only once.
 */

#include <stdlib.h>
#include <string.h>

/* ------------------------------------------------------------------ */
/* static data attached to the function                                 */
/* ------------------------------------------------------------------ */
static seq_t   *cache   = NULL;   /* array of computed values                */
static size_t  cap     = 0;       /* capacity of the allocated array         */
static size_t  last    = (size_t)-1;   /* highest index for which a value is set */

/* ------------------------------------------------------------------ */
/* Helper: resize the cache if required, and initialise new entries   */
/* ------------------------------------------------------------------ */
static void ensure_capacity(idx_t n) {
    size_t wanted = (size_t)n + 1;         /* need indices 0..n inclusive   */
    if (cap >= wanted) return;            /* already big enough             */

    seq_t *new_cache = (seq_t *)realloc(cache, wanted * sizeof(seq_t));
    if (!new_cache)                       /* allocation failed – leave old   */
        return;

    /* initialise newly added slots to zero */
    if (cap < wanted)
        memset((void *)(new_cache + cap), 0, (wanted - cap) * sizeof(seq_t));

    cache = new_cache;
    cap   = wanted;

    /* initialise the special first 12 values */
    if (cap >= 1) cache[0] = (seq_t)1;             /* a(0) = 1 */
    /* for 1..5 already 0 */
    if (cap > 6)
        for (size_t i = 6; i <= 11 && i < cap; ++i)
            cache[i] = (seq_t)1;                 /* a(6)..a(11) = 1 */
}

/* ------------------------------------------------------------------ */
/* Main routine                                                          */
/* ------------------------------------------------------------------ */
seq_t a(idx_t n)
{
    /* First time: build the cache to index 11 */
    if ((size_t)n + 1 > cap) {
        ensure_capacity(11);
        last = 11;
    }

    /* Return already known value */
    if (n <= (idx_t)last)
        return cache[(size_t)n];

    /* Grow cache to required size */
    if ((size_t)n + 1 > cap)
        ensure_capacity((idx_t)n);

    /* Compute missing values */
    for (size_t i = last + 1; i <= (size_t)n; ++i) {
        /* Base cases (i <= 11) are already set; the recurrence applies for i >= 12 */
        if (i < 12) {
            /* values 0..11 are already correct */
            continue;
        }
        cache[i] = cache[i-6] + cache[i-7] + cache[i-8]
                    + cache[i-9] + cache[i-10] + cache[i-11];
    }
    last = (size_t)n;
    return cache[(size_t)n];
}
