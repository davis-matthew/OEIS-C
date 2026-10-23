/*
 * A185688  First differences of A060819(n-4)*A060819(n).
 * https://oeis.org/A185688
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign, easy
 * Formula:   not stated
 * Range:     i8 n <= 13, i16 n <= 187, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A185688
   Formula: a(n) = 3*a(n-4) - 3*a(n-8) + a(n-12)
   Initial values (1‑based):
   a(1)= 2, a(2)=-2, a(3)= 3, a(4)= 5,
   a(5)=-2, a(6)=18, a(7)=-19, a(8)=43,
   a(9)=-30, a(10)=62, a(11)=-71, a(12)=111
   This recurrence is valid for all n ≥ 13.
*/
#include <stdlib.h>

/* Global cache: cache[0] is unused so that cache[n] holds a(n). */
static seq_t *cache = NULL;      /* cache array */
static idx_t cached_n = 0;       /* largest n currently stored */

/* Initial values for indices 1..12 */
static const seq_t init_vals[13] = {
    0, /* index 0 unused */
    2,   /* a(1) */
    -2,  /* a(2) */
    3,   /* a(3) */
    5,   /* a(4) */
    -2,  /* a(5) */
    18,  /* a(6) */
    -19, /* a(7) */
    43,  /* a(8) */
    -30, /* a(9) */
    62,  /* a(10) */
    -71, /* a(11) */
    111  /* a(12) */
};

/* Expand the cache up to at least n (n ≥ 1). */
static void ensure_cache(idx_t n)
{
    if (n <= cached_n || n < 1)
        return;

    /* New size (index 0 unused) */
    size_t new_size = (size_t)n + 1;
    /* Reallocate memory */
    seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
    if (!new_cache) {
        /* Allocation failed – leave cache unchanged. */
        return;
    }
    cache = new_cache;

    /* Fill new entries.  For indices <=12 use the table, otherwise use
       the linear recurrence. */
    for (size_t i = cached_n + 1; i <= (size_t)n; ++i) {
        if (i <= 12) {
            cache[i] = init_vals[i];
        } else {
            cache[i] = (seq_t)3 * cache[i - 4]
                       - (seq_t)3 * cache[i - 8]
                       + cache[i - 12];
        }
    }
    cached_n = n;
}

/* External function required by the specification. */
seq_t a(idx_t n)
{
    if (n < 1)
        return 0;  /* undefined for n < 1; return 0 as placeholder */

    ensure_cache(n);
    return cache[(size_t)n];
}
