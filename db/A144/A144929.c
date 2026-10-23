/*
 * A144929  Numbers n such that there exists an integer k with (n+1)^3 - n^3 =
 *          7*k^2.
 * https://oeis.org/A144929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 10, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A144929: Numbers n such that (n+1)^3‑n^3 = 7*k^2.
   Recurrence: a(n+2) = 110*a(n+1) – a(n) + 54,
   with a(1) = 1, a(2) = 166. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* offset 1: ignore n = 0 */

    static seq_t *cache = NULL;   /* memoised terms, cache[0] unused */
    static size_t cap   = 0;      /* size of allocated array (>= last+1) */
    static size_t last  = 0;      /* highest index whose value is known */

    /* Ensure at least indices 1 and 2 are available. */
    if (last < 2) {
        size_t need = 3;                  /* indices 0..2 inclusive */
        if (cap < need) {
            size_t newcap = (cap ? cap : 16);
            while (newcap < need) newcap <<= 1;
            seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
            if (!tmp) abort();
            cache = tmp;
            cap   = newcap;
        }
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)166;
        last    = 2;
    }

    /* Grow the cache if the requested index is beyond current capacity. */
    size_t needed = (size_t)n + 1;          /* indices 0..n inclusive */
    if (cap < needed) {
        size_t newcap = (cap ? cap : 16);
        while (newcap < needed) newcap <<= 1;
        seq_t *tmp = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) abort();
        cache = tmp;
        cap   = newcap;
    }

    /* Compute any missing terms using the recurrence. */
    for (size_t i = last + 1; i <= (size_t)n; ++i) {
        seq_t v = (seq_t)110 * cache[i - 1] - cache[i - 2] + (seq_t)54;
        cache[i] = v;
    }
    last = (size_t)n;

    return cache[(size_t)n];
}
